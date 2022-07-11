/*****************************************************************//**
 * \file   opengl.h
 * \brief  OpenGL context class definition module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include <glad/glad.h>

#include "core/render/render_context.h"

namespace scl
{
    /* OpenGL context */
    class gl : public render_context
    {
#ifdef SCL_PLATFORM_WINDOWS
    private: /* OpenGL context data. */
        static HGLRC hGLRC;
        static HDC hDC;
        static const HWND *hWnd;

    private:
        /**
         * Get OpenGL primitive type by mesh type function.
         * 
         * \param MeshType - mesh type to get OpenGL primitive type according to.
         * \return OpenGL primitive type.
         */
        inline static GLenum GetGLPrimitiveType(mesh_type MeshType);

    public:
        /**
         * Render system type constructor.
         *
         * \param hAppWnd - window handle.
         * \param W, H - window size.
         * \param VSync - vertical syncronisation enable flag.
         * \return None.
         */
        static void CreateContext(const HWND &hAppWnd, int W, int H, bool VSync);
#else /* !SCL_PLATFORM_WINDOWS */
#   error Other platforms currently dont support OpenGL
#endif
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
         * Frame clear color setter function.
         *
         * \param Color - new frame clear color.
         * \return None.
         */
        void SetClearColor(const vec4 &Color) override;

        /**
         * Setting render wire frame mode function.
         *
         * \param IsWireframe - wireframe rendering flag.
         * \return None.
         */
        void SetWireframeMode(bool IsWireframe) override;

        /**
         * Clear current render target function.
         *
         * \param None.
         * \return None.
         */
        void Clear() override;

        /**
         * Swap frame buffers function.
         *
         * \param None.
         * \return None.
         */
        void SwapBuffers() override;

        /**
         * Conetxt resize function.
         *
         * \param Width, Height - new width and hight of applicatino window.
         * \return None.
         */
        void Resize(int Width, int Height) override;

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

        /**
         * Set rendering context to main window.
         * 
         * \param None.
         * \return None.
         */
        void SetContextCurrent() const;

    public: /* Sculpto library built-in backend API specific rendering objects getter function. */
        /* OpenGL specific single color material shader getter function. */
        shared<shader_program> GetSingleColorMaterialShader() const override;

        /* OpenGL specific phong lighting model material shader getter function. */
        shared<shader_program> GetPhongMaterialShader() const override;

        /* OpenGL specific shadow pass shader getter function. */
        shared<shader_program> GetShadowPassShader() const override;

        /* OpenGL specific full viewport mesh with tone mapping shader. */
        shared<shader_program> GetToneMappingPassShader() const override;

        /* Backend API specific gaussian blur pass shader getter function. */
        shared<shader_program> GetGaussianBlurPassShader() const override;
    };
}
