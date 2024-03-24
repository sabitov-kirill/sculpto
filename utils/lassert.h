//
// Created by ne1mnn on 2/28/2024.
//

#ifndef ASSERT_H
#define ASSERT_H

#include "logger/logger.h"
#include "platform.h"

namespace scl {

extern std::unique_ptr<logger> assert_logger;

template <typename T, typename... Args>
void assert_true(T&& value, std::format_string<Args...> fmt, Args&&... args) {
  if constexpr (platform::is_debug_v) {
    if (!value) {
      assert_logger->vlog(logger::message_type::ERR, fmt.get(), std::make_format_args(args...));
      std::abort();
    }
  }
}

template <typename T>
void assert_true(T&& value) {
  assert_true(std::forward<T>(value), "Expected true");
}

void assert_eq(auto&& left, auto&& right) {
  assert_true(left == right, "Expected {} to be equal to {}", left, right);
}

}

#endif //ASSERT_H
