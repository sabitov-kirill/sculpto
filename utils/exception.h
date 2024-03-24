//
// Created by ne1mnn on 3/4/2024.
//

#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace scl {

class exception : std::exception {
 public:
  exception() noexcept = default;

  explicit exception(std::string message) : _message(std::move(message)) {}

  exception(const exception&) noexcept = default;
  exception(exception&&) noexcept = default;
  exception& operator=(const exception&) noexcept = default;
  exception& operator=(exception&&) noexcept = default;

  ~exception() noexcept override = default;

 public: /* Methods */
  [[nodiscard]] const char* what() const noexcept override { return _message.c_str(); }

 private: /* Fields */
  std::string _message{};
};

}  // namespace scl

#endif  // EXCEPTION_H
