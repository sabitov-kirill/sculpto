//
// Created by ne1mnn on 2/27/2024.
//

#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include "logger.h"

namespace scl {

class console_logger final : public logger {
  using logger::logger;

 protected:
  void write_log(message_type type, const std::string& log) const override;
  void write_log(message_type type, const std::wstring& log) const override;

 private:
  static void write_log_impl(const message_type type, auto&& out, auto&& error_out, auto&& log) {
    auto& stream = type == message_type::ERR ? error_out : out;
    stream << log << std::endl;
  }
};

}  // namespace scl

#endif  // CONSOLE_LOGGER_H
