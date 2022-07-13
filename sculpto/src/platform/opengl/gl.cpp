/*****************************************************************//**
 * \file   gl.cpp
 * \brief  OpenGL context implementation module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "core/application/application.h"
#include "core/render/primitives/vertex_array.h"
#include "gl.h"

#ifdef SCL_PLATFORM_WINDOWS

/* OpenGL WGL rendering context creation definitions. */
typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

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
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC hGLRC = wglCreateContextAttribsARB(hDC, 0, context_attribs);
    SCL_CORE_ASSERT(hGLRC, "Failed to create OpenGL context.");
    SCL_CORE_ASSERT(wglMakeCurrent(hDC, hGLRC), "Failed to set rendering context to main window.");
    SCL_CORE_ASSERT(gladLoadGL(), "Failed to load OpenGL functions pointers with glad.");

    return hGLRC;
}

#endif /* !SCL_PLATFORM_WINDOWS */

const scl::vec4 &scl::gl::GetClearColor() const
{
    return ClearColor;
}

bool scl::gl::GetWireframeMode() const
{
    return IsWireframe;
}

scl::render_cull_face_mode scl::gl::GetCullingMode() const
{
    return CullingMode;
}

void scl::gl::SetClearColor(const vec4 &ClearColor)
{
    this->ClearColor = ClearColor;
    glClearColor(SCL_VEC_XYZW(ClearColor));
}

void scl::gl::SetWireframeMode(bool IsWireframe)
{
    this->IsWireframe = IsWireframe;
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframe ? GL_LINE : GL_FILL);
}

void scl::gl::SetCullingMode(render_cull_face_mode CullingMode)
{
    this->CullingMode = CullingMode;
    switch (CullingMode)
    {
    case scl::render_cull_face_mode::OFF:   glDisable(GL_CULL_FACE); return;
    case scl::render_cull_face_mode::BACK:  glEnable(GL_CULL_FACE); glCullFace(GL_BACK); return;
    case scl::render_cull_face_mode::FRONT: glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); return;
    }

    SCL_CORE_ASSERT(0, "Unknown culling mode!");
}

GLenum scl::gl::GetGLPrimitiveType(mesh_type MeshType)
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

GLenum scl::gl::GetGLShaderVariableType(shader_variable_type Type)
{
    switch (Type)
    {
    case scl::shader_variable_type::BOOL:
        return GL_BOOL;

    case scl::shader_variable_type::INT:
    case scl::shader_variable_type::INT2:
    case scl::shader_variable_type::INT3:
    case scl::shader_variable_type::INT4:
        return GL_INT;

    case scl::shader_variable_type::FLOAT:
    case scl::shader_variable_type::FLOAT2:
    case scl::shader_variable_type::FLOAT3:
    case scl::shader_variable_type::FLOAT4:
    case scl::shader_variable_type::MATR4:
        return GL_FLOAT;
    }

    SCL_CORE_ASSERT(0, "Unknown shader veriable type.");
    return GLenum();
}

static void APIENTRY glDebugOutput(UINT Source, UINT Type, UINT Id, UINT Severity,
                                   INT Length, const CHAR *Message, const VOID *UserParam);

void scl::gl::CreateContext(const HWND &hAppWnd, int W, int H, bool VSync)
{
#ifdef SCL_PLATFORM_WINDOWS
    hWnd = &hAppWnd;
    hDC = GetDC(hAppWnd);
    glInitialiseExtentions(hDC);
    hGLRC = glInitialiseContext(hDC);
#endif /* !SCL_PLATFORM_WINDOWS */

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
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(render_context::MESH_RESTART_INDEX);
}

void scl::gl::Init()
{
    application &app = application::Get();
    const window &win = app.GetWindow();
    const window::data win_data = win.GetWindowData();

    this->CreateContext(win.GetHandle(), win_data.Width, win_data.Height, false);
    SCL_CORE_SUCCES("OpenGL {}.{} context created.", GLVersion.major, GLVersion.minor);
}

void scl::gl::Close()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hGLRC);
    ReleaseDC(*hWnd, hDC);
}

void scl::gl::SwapBuffers()
{
    /* Another way (not sure):
     * wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
     */
    ::SwapBuffers(hDC);
}

void scl::gl::DrawIndices(const shared<vertex_array> &VertexArray)
{
    VertexArray->Bind();
    glDrawElements(
        GetGLPrimitiveType(VertexArray->GetType()),
        VertexArray->GetIndexBuffer()->GetCount(),
        GL_UNSIGNED_INT,
        nullptr
    );
}

void scl::gl::DrawIndicesInstanced(const shared<vertex_array> &VertexArray, int InstanceCount)
{

    glDrawElementsInstanced(
        GetGLPrimitiveType(VertexArray->GetType()),
        VertexArray->GetIndexBuffer()->GetCount(),
        GL_UNSIGNED_INT,
        nullptr,
        InstanceCount
    );
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
    case GL_DEBUG_SOURCE_API:             len += sprintf_s(Buf + len, 10000, "Source: API\n");             break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   len += sprintf_s(Buf + len, 10000, "Source: Window System\n");   break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: len += sprintf_s(Buf + len, 10000, "Source: Shader Compiler\n"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     len += sprintf_s(Buf + len, 10000, "Source: Third Party\n");     break;
    case GL_DEBUG_SOURCE_APPLICATION:     len += sprintf_s(Buf + len, 10000, "Source: Application\n");     break;
    case GL_DEBUG_SOURCE_OTHER:           len += sprintf_s(Buf + len, 10000, "Source: Other\n");           break;
    }
    len += sprintf_s(Buf + len, 10000, "\n");

    switch (Type)
    {
    case GL_DEBUG_TYPE_ERROR:               len += sprintf_s(Buf + len, 10000, "Type: Error");                break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: len += sprintf_s(Buf + len, 10000, "Type: Deprecated Behaviour"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  len += sprintf_s(Buf + len, 10000, "Type: Undefined Behaviour");  break;
    case GL_DEBUG_TYPE_PORTABILITY:         len += sprintf_s(Buf + len, 10000, "Type: Portability"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         len += sprintf_s(Buf + len, 10000, "Type: Performance"); break;
    case GL_DEBUG_TYPE_MARKER:              len += sprintf_s(Buf + len, 10000, "Type: Marker");      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          len += sprintf_s(Buf + len, 10000, "Type: Push Group");  break;
    case GL_DEBUG_TYPE_POP_GROUP:           len += sprintf_s(Buf + len, 10000, "Type: Pop Group");   break;
    case GL_DEBUG_TYPE_OTHER:               len += sprintf_s(Buf + len, 10000, "Type: Other");       break;
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
