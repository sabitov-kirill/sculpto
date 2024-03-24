//
// Created by ne1mnn on 2/27/2024.
//

#ifndef PLATFORM_H
#define PLATFORM_H

namespace scl {

namespace platform {

enum class os { WINDOWS, LINUX, MACOS, ANDROID, IOS, EMSCRIPTEN, UNKNOWN };

constexpr os get_platform() noexcept {
#if defined(__linux__) && !defined(__android__)
  return os::LINUX;
#elif defined(__ANDROID__)
  return os::ANDROID;
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
  return os::WINDOWS;
#elif defined(__APPLE__) && defined(IPHONE) && !defined(__MACH__)
  return os::IOS;
#elif defined(__APPLE__) && !defined(IPHONE) && defined(__MACH__)
  return os::MACOS;
#elif defined(__EMSCRIPTEN__)
  return os::EMSCRIPTEN;
#else
  return os::UNKNOWN;
#endif
}

constexpr os current_os_v = get_platform();

template <os Expected>
constexpr bool is_os_v = current_os_v == Expected;
constexpr bool is_windows_v = is_os_v<os::WINDOWS>;
constexpr bool is_linux_v = is_os_v<os::LINUX>;
constexpr bool is_macos_v = is_os_v<os::MACOS>;
constexpr bool is_ios_v = is_os_v<os::IOS>;
constexpr bool is_android_v = is_os_v<os::ANDROID>;
constexpr bool is_emcscripten_v = is_os_v<os::EMSCRIPTEN>;

enum class compiler { MSVC, GCC, CLANG, EMSCRIPTEN, UNKNOWN };

enum class build_mode { DEBUG, RELEASE, UNKNOWN };

constexpr build_mode get_build_mode() noexcept {
#if defined(NDEBUG) && !defined(_DEBUG)
  return build_mode::RELEASE;
#else
  return build_mode::DEBUG;
#endif
}

constexpr build_mode current_build_mode_v = get_build_mode();

template <build_mode Expected>
constexpr bool is_build_mode_v = current_build_mode_v == Expected;
constexpr bool is_debug_v = is_build_mode_v<build_mode::DEBUG>;
constexpr bool is_release_v = is_build_mode_v<build_mode::RELEASE>;

}  // namespace platform

}  // namespace scl

#endif  // PLATFORM_H
