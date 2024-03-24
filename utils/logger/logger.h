//
// Created by ne1mnn on 2/27/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <format>
#include <string>
#include <unordered_map>

namespace scl {

class logger {
 public: /* Helper types */
  enum class message_type { INFO, WARNING, ERR };

 public: /* Constructors and destructors */
  logger();
  explicit logger(std::string name);

  virtual ~logger() = default;

 public: /* Methods */
  void vlog(const message_type type, const std::string_view fmt, const std::format_args args) const {
    const auto formatted = std::vformat(fmt, args);
    const auto now = std::chrono::system_clock::now();
    const auto log = std::format("{:%H:%M:%S} [{}] {}: {}", now, _message_type_str.at(type), _name, formatted);

    write_log(type, log);
  }

  template <typename... Args>
  void log(const message_type type, std::format_string<Args...> fmt, Args&&... args) const {
    vlog(type, fmt.get(), std::make_format_args(args...));
  }

  template <typename... Args>
  void info(std::format_string<Args...> fmt, Args&&... args) const {
    vlog(message_type::INFO, fmt.get(), std::make_format_args(args...));
  }

  template <typename... Args>
  void warning(std::format_string<Args...> fmt, Args&&... args) const {
    vlog(message_type::WARNING, fmt.get(), std::make_format_args(args...));
  }

  template <typename... Args>
  void error(std::format_string<Args...> fmt, Args&&... args) const {
    vlog(message_type::ERR, fmt.get(), std::make_format_args(args...));
  }

 protected:
  virtual void write_log(message_type type, const std::string& log) const = 0;
  virtual void write_log(message_type type, const std::wstring& log) const = 0;

 public: /* Data getters and setters */
  [[nodiscard]] virtual const std::string& get_name() const;

  virtual void set_name(std::string name);

 private: /* Data */
  static const std::unordered_map<message_type, std::string> _message_type_str;

  std::string _name{};
};

}  // namespace scl

#endif  // LOGGER_H
