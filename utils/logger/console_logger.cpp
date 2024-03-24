//
// Created by ne1mnn on 2/27/2024.
//

#include "console_logger.h"

#include <iostream>

void scl::console_logger::write_log(const message_type type, const std::string& log) const {
  write_log_impl(type, std::cout, std::cerr, log);
}

void scl::console_logger::write_log(const message_type type, const std::wstring& log) const {
  write_log_impl(type, std::wcout, std::wcerr, log);
}