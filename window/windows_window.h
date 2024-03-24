//
// Created by ne1mnn on 2/27/2024.
//

#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include <windows.h>
#include <windowsx.h>

#include <string>

#include "lassert.h"

namespace scl {

template <typename T>
class windows_window {
 public: /* Constructors and destructors */
  explicit windows_window(std::wstring name, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT)
      : _name(std::move(name)), _width(width), _height(height) {}

 public: /* Methods */
  void update() {
    if (!_is_initialized) {
      create_window_class();
      create_window();
      show_window();
      _is_initialized = true;
    }

    MSG message{};
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) return;

      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  void on_create() {}

  void on_close() {}

  void on_resize(int, int) {}

  void on_erase_background(HDC) {}

  void on_draw(HDC) {}

  void on_mouse_move(int x, int y, UINT key_flags) {}

  void on_mouse_wheel(int x, int y, int z, UINT keys) {}

  void on_mouse_up(int x, int y, UINT keys) {}

  void on_mouse_down(bool is_double_click, int x, int y, UINT keys) {}

  void on_key(UINT code, bool is_down, int repeat_count, UINT flags) {}

 private:
  void create_window_class() const {
    WNDCLASSW window_class{};

    window_class.lpszClassName = _window_class_name;
    window_class.lpszMenuName = nullptr;
    window_class.style = CS_VREDRAW | CS_HREDRAW;
    window_class.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    window_class.hCursor = LoadCursor(_instance, IDC_ARROW);
    window_class.hIcon = LoadIcon(_instance, IDI_APPLICATION);
    window_class.hInstance = _instance;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = sizeof(T *);
    window_class.lpfnWndProc = window_procedure;

    assert_true(RegisterClassW(&window_class), "Window class creation failed.");
  }

  void create_window() {
    // Create window with comment to each parameter
    assert_true(CreateWindowExW(0,                    // Extended window style
                                _window_class_name,   // Window class name
                                _name.c_str(),        // Window title
                                WS_OVERLAPPEDWINDOW,  // Window style
                                CW_USEDEFAULT,        // X position
                                CW_USEDEFAULT,        // Y position
                                _width,               // Width
                                _height,              // Height
                                nullptr,              // Parent window
                                nullptr,              // Menu
                                _instance,            // Instance
                                this                  // Pointer to window
                                ),
                "Window creation failed.");
  }

  void show_window() const { ShowWindow(_window, SW_SHOWNORMAL); }

  static LRESULT window_procedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NCCREATE) {
      const auto cs = reinterpret_cast<CREATESTRUCT *>(lParam);
      auto that = static_cast<windows_window *>(cs->lpCreateParams);

      that->_window = window;
      SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(that));
    } else if (message == WM_NCDESTROY) {
      SetWindowLongPtrW(window, GWLP_USERDATA, 0);
    } else if (auto that = reinterpret_cast<T *>(GetWindowLongPtrW(window, GWLP_USERDATA))) {
      return that->message_handler(message, wParam, lParam);
    }

    return DefWindowProcW(window, message, wParam, lParam);
  }

  T &derived() { return *static_cast<T *>(this); }

 protected: /* WinAPI message handlers */
  LRESULT message_handler(const UINT message, const WPARAM wParam, const LPARAM lParam) {
    switch (message) {
      HANDLE_MSG(_window, WM_CREATE, message_create);
      HANDLE_MSG(_window, WM_SIZE, message_size);
      HANDLE_MSG(_window, WM_CLOSE, message_close);
      HANDLE_MSG(_window, WM_DESTROY, message_destroy);
      HANDLE_MSG(_window, WM_ACTIVATE, message_activate);
      HANDLE_MSG(_window, WM_TIMER, message_timer);
      HANDLE_MSG(_window, WM_ERASEBKGND, message_erase_background);
      HANDLE_MSG(_window, WM_PAINT, message_paint);
      HANDLE_MSG(_window, WM_MOUSEMOVE, message_mouse_move);
      HANDLE_MSG(_window, WM_MOUSEWHEEL, message_mouse_wheel);
      HANDLE_MSG(_window, WM_LBUTTONDOWN, message_mouse_down);
      HANDLE_MSG(_window, WM_RBUTTONDOWN, message_mouse_down);
      HANDLE_MSG(_window, WM_LBUTTONUP, message_mouse_up);
      HANDLE_MSG(_window, WM_RBUTTONUP, message_mouse_up);
      HANDLE_MSG(_window, WM_KEYDOWN, message_key);
      HANDLE_MSG(_window, WM_KEYUP, message_key);

      default:
        return DefWindowProc(_window, message, wParam, lParam);
    }
  }

  bool message_create(HWND, CREATESTRUCT *cs) {
    derived().on_create();
    return true;
  }

  void message_size(HWND, UINT state, int w, int h) {
    _width = w;
    _height = h;
    derived().on_size(w, h);
  }

  bool message_close(HWND) {
    DestroyWindow(_window);
    return false;
  }

  void message_destroy(HWND) { derived().on_close(); }

  void message_activate(HWND, UINT reason, HWND window_deact, bool is_minimized) {}

  void message_timer(HWND, UINT id) {}

  bool message_erase_background(HWND, HDC draw_context) {
    derived().on_erase_background(draw_context);
    return true;
  }

  void message_paint(HWND) {
    PAINTSTRUCT ps;
    HDC draw_context = BeginPaint(_window, &ps);

    // Create a bitmap big enough for our client rectangle.
    RECT rc;
    GetClientRect(_window, &rc);
    HDC hdcMem = CreateCompatibleDC(ps.hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, rc.right - rc.left, rc.bottom - rc.top);
    auto hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, hbmMem));

    // Erase the background.
    HBRUSH hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    FillRect(hdcMem, &rc, hbrBkGnd);
    DeleteObject(hbrBkGnd);

    // Draw
    derived().on_draw(hdcMem);

    // Done with off-screen bitmap and DC.
    BitBlt(ps.hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
    EndPaint(_window, &ps);
  }

  void message_mouse_move(HWND, int x, int y, UINT key_flags) { derived().on_mouse_move(x, y, key_flags); }

  void message_mouse_wheel(HWND, int x, int y, int z, UINT keys) { derived().on_mouse_wheel(x, y, z, keys); }

  void message_mouse_down(HWND, bool is_double_click, int x, int y, UINT keys) {
    derived().on_mouse_down(is_double_click, x, y, keys);
  }

  void message_mouse_up(HWND, int x, int y, UINT keys) { derived().on_mouse_up(x, y, keys); }

  void message_key(HWND, UINT code, bool is_down, int repeat_count, UINT flags) {
    derived().on_mouse_down(code, is_down, repeat_count, flags);
  }

 public: /* Data getters and setters */
  [[nodiscard]] const std::wstring &get_name() const { return _name; }

  [[nodiscard]] std::pair<int, int> get_size() const { return {_width, _height}; }

  [[nodiscard]] HWND get_handle() const { return _window; }

  [[nodiscard]] HINSTANCE get_instance() const { return _instance; }

  [[nodiscard]] bool is_initialized() const { return _is_initialized; }

  [[nodiscard]] static constexpr const wchar_t *get_window_class_name() { return _window_class_name; }

  void set_name(std::wstring name) {
    _name = std::move(name);
    SetWindowTextW(_window, _name.c_str());
  }

  void set_size(const int width, const int height) {
    _width = width;
    _height = height;
    SetWindowPos(_window, nullptr, 0, 0, _width, _height, SWP_NOMOVE | SWP_NOZORDER);
  }

 private: /* Data */
  HWND _window{};
  HINSTANCE _instance{GetModuleHandleW(nullptr)};
  bool _is_initialized{false};

  static constexpr wchar_t _window_class_name[] = L"SCL Window Class Name";
  std::wstring _name{};
  int _width{}, _height{};
};

}  // namespace scl

#endif  // WINDOWS_WINDOW_H
