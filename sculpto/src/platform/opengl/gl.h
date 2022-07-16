/*****************************************************************//**
 * \file   opengl.h
 * \brief  OpenGL context class definition module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#define GLEW_STATIC
#include <glew.h>

#include "core/render/render_context.h"

namespace scl
{
    /* Classes declaration. */
    class shader_program;

    /* OpenGL context */
    class gl : public render_context
    {

    private: /* OpenGL context data. */
        vec4 ClearColor { 0 };
        bool IsWireframe {};
        bool IsVSync {};
        render_cull_face_mode CullingMode { render_cull_face_mode::BACK };

        static shared<shader_program> single_color_material_shader;
        static shared<shader_program> phong_geometry_shader;
        static shared<shader_program> phong_lighting_shader;
        static shared<shader_program> shadow_pass_shader;
        static shared<shader_program> tone_mapping_pass_shader;
        static shared<shader_program> gaussian_blur_pass_shader;
        static shared<shader_program> texture_add_pass_shader;

#ifdef SCL_PLATFORM_WINDOWS
        HGLRC hGLRC;
        HDC hDC;
        const HWND *hWnd;
#else /* !SCL_PLATFORM_WINDOWS */
#   error Other platforms currently dont support OpenGL
#endif

    public: /* OpenGL data getter/setter functions. */
        /* Frame clear color setter function. */
        const vec4 &GetClearColor() const override;
        /* Render wire frame mode setter function. */
        bool GetWireframeMode() const override;
        /* Render culling mode setter function. */
        render_cull_face_mode GetCullingMode() const override;
        /* Render virtual syncronisation flag getter function. */
        bool GetVSync() const override;

        /* Frame clear color setter function. */
        void SetClearColor(const vec4 &ClearColor) override;
        /* Render wire frame mode setter function. */
        void SetWireframeMode(bool IsWireframe) override;
        /* Render culling mode setter function. */
        void SetCullingMode(render_cull_face_mode CullingMode) override;
        /* Render virtual syncronisation flag setter function. */
        void SetVSync(bool VSync);

        /**
         * Get OpenGL primitive type by mesh type function.
         *
         * \param MeshType - mesh type to get OpenGL primitive type according to.
         * \return OpenGL primitive type.
         */
        static GLenum GetGLPrimitiveType(mesh_type MeshType);

        /**
         * Get OpenGL shader variable type function.
         * 
         * \param Type - top-level api shader variable.
         * \return OpenGL shader variable type.
         */
        static GLenum GetGLShaderVariableType(shader_variable_type Type);

    public: /* OpenGL context methods. */
        /**
         * Render system type constructor.
         *
         * \param hAppWnd - window handle.
         * \param W, H - window size.
         * \param VSync - vertical syncronisation enable flag.
         * \return None.
         */
        void CreateContext(const HWND &hAppWnd, int W, int H, bool VSync);

        /**
         * Render context initialisation function.
         *
         * \param None.
         * \return None.
         */
        void Init() override;

        /**
         * Render context deinitialisation function.
         *
         * \param None.
         * \return None.
         */
        void Close() override;

        /**
         * Swap frame buffers function.
         *
         * \param None.
         * \return None.
         */
        void SwapBuffers() override;

        /**
         * Draw vertices function.
         *
         * \param VertexArray - mesh, containing vertices and vertex indices to draw.
         * \return None.
         */
        void DrawIndices(const shared<vertex_array> &VertexArray) override;

        /**
         * Draw vertices instanced function.
         *
         * \param VertexArray - mesh, containing vertices and vertex indices to draw.
         * \return None.
         */
        void DrawIndicesInstanced(const shared<vertex_array> &VertexArray, int InstanceCount) override;

    public: /* Sculpto library built-in backend API specific rendering objects getter function. */
        /* OpenGL specific single color material shader getter function. */
        shared<shader_program> GetSingleColorMaterialShader() const override;
        /* OpenGL specific phong lighint model shader for geometry pass getter function. */
        shared<shader_program> GetPhongGeometryShader() const override;
        /* OpenGL specific phong lighint model shader for lighting pass getter function. */
        shared<shader_program> GetPhongLightingShader() const override;
        /* OpenGL specific shadow pass shader getter function. */
        shared<shader_program> GetShadowPassShader() const override;
        /* OpenGL specific full viewport mesh with tone mapping shader. */
        shared<shader_program> GetToneMappingPassShader() const override;
        /* OpenGL specific gaussian blur pass shader getter function. */
        shared<shader_program> GetGaussianBlurPassShader() const override;
        /* OpenGL specific gaussian blur pass shader getter function. */
        shared<shader_program> GetTextureAddPassShader() const override;
    };
}
