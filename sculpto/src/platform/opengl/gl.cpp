/*****************************************************************//**
 * \file   gl.cpp
 * \brief  OpenGL context implementation module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "core/application/application.h"
#include "core/render/primitives/mesh.h"
#include "gl.h"

/* Debug output function. */
static void APIENTRY glDebugOutput(UINT Source, UINT Type, UINT Id, UINT Severity,
                                   INT Length, const CHAR *Message, const VOID *UserParam);

#ifdef SCL_PLATFORM_WINDOWS

/* OpenGL WGL rendering context creation definitions. */
typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

/* OpenGL WGL pixel format configuration definitions. */
typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

/* OpenGL WGL initialisation functions. */
void glInitialiseExtentions(HDC hDC)
{
    // Temporery context creation
    PIXELFORMATDESCRIPTOR pfd {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;

    int pixel_format = ChoosePixelFormat(hDC, &pfd);
    SCL_CORE_ASSERT(pixel_format, "Failed to find a suitable pixel format.");
    SCL_CORE_ASSERT(SetPixelFormat(hDC, pixel_format, &pfd), "Failed to set the pixel format.");

    HGLRC hTmpGLRC = wglCreateContext(hDC);
    SCL_CORE_ASSERT(hTmpGLRC, "Failed to create temporery OpenGL rendering context.");
    SCL_CORE_ASSERT(wglMakeCurrent(hDC, hTmpGLRC), "Failed to activate temporery OpenGL rendering context.");

    // Load functions pointers
    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type *)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type *)wglGetProcAddress("wglChoosePixelFormatARB");

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hTmpGLRC);
}

HGLRC glInitialiseContext(HDC hDC)
{
    // Choose pixel format
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         32,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(hDC, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    SCL_CORE_ASSERT(num_formats, "Failed to set the OpenGL pixel format.");

    PIXELFORMATDESCRIPTOR pfd {};
    DescribePixelFormat(hDC, pixel_format, sizeof(pfd), &pfd);
    SCL_CORE_ASSERT(SetPixelFormat(hDC, pixel_format, &pfd), "Failed to set the OpenGL pixel format.");

    // Setup rendering context
    int context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC hGLRC = wglCreateContextAttribsARB(hDC, 0, context_attribs);
    SCL_CORE_ASSERT(hGLRC, "Failed to create OpenGL context.");
    SCL_CORE_ASSERT(wglMakeCurrent(hDC, hGLRC), "Failed to set rendering context to main window.");
    SCL_CORE_ASSERT(gladLoadGL(), "Failed to load OpenGL functions pointers with glad.");

    return hGLRC;
}

/* OpenGL conext static variables. */
HGLRC scl::gl::hGLRC;
HDC scl::gl::hDC;
const HWND *scl::gl::hWnd;

inline GLenum scl::gl::GetGLPrimitiveType(mesh_type MeshType)
{

    switch (MeshType)
    {
    case scl::mesh_type::LINES:        return GL_LINES;
    case scl::mesh_type::POINTS:       return GL_POINTS;
    case scl::mesh_type::PATCHES:      return GL_PATCHES;
    case scl::mesh_type::TRIANGLES:    return GL_TRIANGLES;
    case scl::mesh_type::TRISTRIP:     return GL_TRIANGLE_STRIP;
    }

    SCL_CORE_ASSERT(0, "Unknown mesh type!");
    return GLenum();
}

/**
 * Render system type constructor.
 *
 * \param hAppWnd - window handle.
 * \param W, H - window size.
 * \param VSync - vertical syncronisation enable flag.
 * \return None.
 */
void scl::gl::CreateContext(const HWND &hAppWnd, int W, int H, bool VSync)
{
    hWnd = &hAppWnd;
    hDC = GetDC(hAppWnd);
    glInitialiseExtentions(hDC);
    hGLRC = glInitialiseContext(hDC);

    // TODO: Add vsync handle.

    // Enable debug callback
#ifdef SCL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    // Set default OpenGL parameters
    glClearColor(0.30f, 0.5f, 0.7f, 1.0f);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(-1);
}

#endif /* !SCL_PLATFORM_WINDOWS */

void scl::gl::Init()
{
    application &app = application::Get();
    const window &win = app.GetWindow();
    const window::data win_data = win.GetWindowData();

#ifdef SCL_PLATFORM_WINDOWS
    this->CreateContext(win.GetHandle(), win_data.Width, win_data.Height, win_data.VSync);
    SCL_CORE_SUCCES("OpenGL {}.{} context created.", GLVersion.major, GLVersion.minor);
#endif /* !SCL_PLATFORM_WINDOWS */
}

void scl::gl::Close()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hGLRC);
    ReleaseDC(*hWnd, hDC);
}

void scl::gl::SetClearColor(const vec4 &Color)
{
    glClearColor(SCL_VEC_XYZW(Color));
}

void scl::gl::SetWireframe(bool IsWireframe)
{
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframe ? GL_LINE : GL_FILL);
}

void scl::gl::BeginPipeline()
{
    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void scl::gl::EndPipeline()
{
    glFlush();
    // glFinish();
}

void scl::gl::SwapBuffers()
{
    ::SwapBuffers(hDC);
    // wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
}

void scl::gl::Resize(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void scl::gl::DrawVerices(const shared<mesh> &Mesh)
{
    const shared<index_buffer> &Indices = Mesh->GetIndexBuffer();
    const shared<vertex_buffer> &Vertices = Mesh->GetVertexBuffer();

    glDrawElements(
        GetGLPrimitiveType(Mesh->GetType()),
        Indices->GetCount(),
        GL_UNSIGNED_INT,
        nullptr
    );
}

void scl::gl::DrawVericesInstanced(const shared<mesh> &Mesh, int InstanceCount)
{
    const shared<index_buffer> &Indices = Mesh->GetIndexBuffer();
    const shared<vertex_buffer> &Vertices = Mesh->GetVertexBuffer();

    glDrawElementsInstanced(
        GetGLPrimitiveType(Mesh->GetType()),
        Indices->GetCount(),
        GL_UNSIGNED_INT,
        nullptr,
        InstanceCount
    );
}

void scl::gl::SetContextCurrent() const
{
    SCL_CORE_ASSERT(wglMakeCurrent(hDC, hGLRC), "Failed to set rendering context to main window.");
}

/**
 * OpenGl Debug output function.
 * 
 * \param Source - source APi or device.
 * \param Type - error type.
 * \param Id - error message id.
 * \param Severity - message severity.
 * \param Length - message text length.
 * \param Message - message text.
 * \param UserParam - user addon parameters pointer.
 * \return None.
 */
static void APIENTRY glDebugOutput(UINT Source, UINT Type, UINT Id, UINT Severity,
                                   INT Length, const CHAR *Message, const VOID *UserParam)
{
    INT len = 0;
    static CHAR Buf[10000] {};

    /* Ignore non-significant error/warning codes */
    if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
        return;

    len += sprintf_s(Buf + len, 10000, "OpenGL debug message (Id - %i):\n%s.\n", Id, Message);
    switch (Source)
    {
    case GL_DEBUG_SOURCE_API:
        len += sprintf_s(Buf + len, 10000, "Source: API\n");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        len += sprintf_s(Buf + len, 10000, "Source: Window System\n");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        len += sprintf_s(Buf + len, 10000, "Source: Shader Compiler\n");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        len += sprintf_s(Buf + len, 10000, "Source: Third Party\n");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        len += sprintf_s(Buf + len, 10000, "Source: Application\n");
        break;
    case GL_DEBUG_SOURCE_OTHER:
        len += sprintf_s(Buf + len, 10000, "Source: Other\n");
        break;
    }
    len += sprintf_s(Buf + len, 10000, "\n");

    switch (Type)
    {
    case GL_DEBUG_TYPE_ERROR:
        len += sprintf_s(Buf + len, 10000, "Type: Error");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        len += sprintf_s(Buf + len, 10000, "Type: Deprecated Behaviour");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        len += sprintf_s(Buf + len, 10000, "Type: Undefined Behaviour");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        len += sprintf_s(Buf + len, 10000, "Type: Portability");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        len += sprintf_s(Buf + len, 10000, "Type: Performance");
        break;
    case GL_DEBUG_TYPE_MARKER:
        len += sprintf_s(Buf + len, 10000, "Type: Marker");
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        len += sprintf_s(Buf + len, 10000, "Type: Push Group");
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        len += sprintf_s(Buf + len, 10000, "Type: Pop Group");
        break;
    case GL_DEBUG_TYPE_OTHER:
        len += sprintf_s(Buf + len, 10000, "Type: Other");
        break;
    }

    switch (Severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        SCL_CORE_ERROR(Buf);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_LOW:
        SCL_CORE_WARN(Buf);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        SCL_CORE_INFO(Buf);
        break;
    }
} /* End of 'glDebugOutput' function */
