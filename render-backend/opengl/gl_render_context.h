//
// Created by ne1mnn on 3/4/2024.
//

#ifndef GL_RENDER_BACKEND_H
#define GL_RENDER_BACKEND_H

#include "platform.h"
#include "base_window.h"
#include "windows_window.h"
#include "primitives/render_context.h"

#include <GL/gl.h>
#include <GL/wgl.h>

namespace scl {

class base_gl_render_context : public render_context {
 public: /* Constructors and destructors */
  base_gl_render_context();

  base_gl_render_context(const base_gl_render_context &) noexcept = delete;
  base_gl_render_context &operator=(const base_gl_render_context &) noexcept = delete;

  base_gl_render_context(base_gl_render_context &&) noexcept = default;
  base_gl_render_context &operator=(base_gl_render_context &&) noexcept = default;

  virtual ~base_gl_render_context() noexcept = default;

 public: /* Data getters and setters */
  [[nodiscard]] const logger &get_logger() const noexcept { return *_logger; }

 private: /* Data */
  std::unique_ptr<logger> _logger;
};

template <platform::os ForOs = platform::os::WINDOWS>
class gl_render_context final : public base_gl_render_context {
 public: /* Methods */
 public:
  template <typename Window>
    requires std::derived_from<Window, windows_window<Window>>
  void init_render_context(const Window &window) {
    static_assert(platform::is_os_v<ForOs>, "Selected wrong os for OpenGL context creation");

    // Pixel format setup
    PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1};
    if (!_draw_context) {
      _draw_context = GetDC(window.get_handle());

      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION | PFD_DOUBLEBUFFER;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 32;
      pfd.cAlphaBits = 8;
      pfd.iLayerType = PFD_MAIN_PLANE;

      auto format_index = ::ChoosePixelFormat(_draw_context, &pfd);
      assert_true(format_index, "Failed to find a suitable pixel format.");

      DescribePixelFormat(_draw_context, format_index, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
      assert_true(SetPixelFormat(_draw_context, format_index, &pfd), "Failed to set the pixel format.");
    }

    auto active_format_index = ::GetPixelFormat(_draw_context);
    assert_true(active_format_index, "Failed to get the active pixel format.");
    assert_true(DescribePixelFormat(_draw_context, active_format_index, sizeof pfd, &pfd),
                "Failed to describe the active pixel format.");
    assert_true((pfd.dwFlags & PFD_SUPPORT_OPENGL) == PFD_SUPPORT_OPENGL, "Pixel format does not support OpenGL.");

    // Setup temporary rendering context
    HGLRC tmp_render_context = wglCreateContext(_draw_context);
    assert_true(tmp_render_context, "Failed to create temporary OpenGL rendering context.");
    assert_true(wglMakeCurrent(_draw_context, tmp_render_context),
                "Failed to activate temporary OpenGL rendering context.");

    // Setup rendering context
    const int context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,    4, WGL_CONTEXT_MINOR_VERSION_ARB, 6, WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0};
    PROC wglCreateContextAttribsARBPtr = ::wglGetProcAddress("wglCreateContextAttribsARB");
    using wglCreateContextAttribsARB = HGLRC (*)(HDC, HGLRC, const int *);
    _render_context = reinterpret_cast<wglCreateContextAttribsARB>(wglCreateContextAttribsARBPtr)(
        _draw_context, nullptr, context_attribs);
    assert_true(_render_context, "Failed to create OpenGL rendering context.");
    assert_true(wglMakeCurrent(_draw_context, _render_context), "Failed to set rendering context to main window.");

    assert_true(wglDeleteContext(tmp_render_context), "Failed to delete temporary rendering context.");
  }

 private:
  HDC _draw_context{};
  HGLRC _render_context{};
};

}  // namespace scl

#endif  // GL_RENDER_BACKEND_H
