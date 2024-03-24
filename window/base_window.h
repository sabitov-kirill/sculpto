//
// Created by ne1mnn on 2/27/2024.
//

#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

#include <concepts>
#include <string>

namespace scl {

template <typename T>
concept base_window = requires (T window, std::wstring name, int width, int height) {
  requires std::constructible_from<T, std::wstring>;
  requires std::constructible_from<T, std::wstring, int, int>;

  { window.on_create() } -> std::same_as<void>;
  { window.on_close() } -> std::same_as<void>;
  { window.on_resize(width, height) } -> std::same_as<void>;

  { window.get_name() } -> std::convertible_to<std::wstring>;
  { window.get_size() } -> std::convertible_to<std::pair<int, int>>;
  { window.set_name(name) } -> std::same_as<void>;
  { window.set_size(width, height) } -> std::same_as<void>;
};

}

#endif  // BASE_WINDOW_H
