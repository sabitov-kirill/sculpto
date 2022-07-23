/*!****************************************************************//*!*
 * \file   render_bridge.h
 * \brief  Render bridge class definition module.
 *         Static class for making calls to low-level render api,
 *         bridge between application high and low level rendering systems.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "render_context.h"

namespace scl
{
    /*! Vertex buffer and arary classes declaration. */
    class vertex_buffer;
    class vertex_array;

    /*! Render bridge class. */
    class render_bridge
    {
    private: /*! Render bridge data. */
        static shared<render_context> RenderContext;

    public: /*! Render bridge data getter/setter functions. */
        /*! Frame clear color getter function. */
        inline static const vec4 &GetClearColor() { return RenderContext->GetClearColor(); }
        /*! Render wire frame mode getter function. */
        inline static bool GetWireframeMode() { return RenderContext->GetWireframeMode(); }
        /*! Render culling mode getter function. */
        inline static render_cull_face_mode GetCullingMode() { return RenderContext->GetCullingMode(); }
        /*! Render depth test mode getter function. */
        inline static bool GetDepthTestMode() { return RenderContext->GetDepthTestMode(); }
        /*! Render virtual syncronisation flag getter function. */
        inline static bool GetVSync() { return RenderContext->GetVSync(); }

        /*! Frame clear color setter function. */
        inline static void SetClearColor(const vec4 &ClearColor) { RenderContext->SetClearColor(ClearColor); }
        /*! Render wire frame mode setter function. */
        inline static void SetWireframeMode(bool IsWireframe) { RenderContext->SetWireframeMode(IsWireframe); }
        /*! Render culling mode setter function. */
        inline static void SetCullingMode(render_cull_face_mode CullingMode) { RenderContext->SetCullingMode(CullingMode); }
        /*! Render depth test mode setter function. */
        inline static void SetDepthTestMode(bool IsDepthTest) { return RenderContext->SetDepthTestMode(IsDepthTest); }
        /*! Render virtual syncronisation flag setter function. */
        inline static void SetVSync(bool VSync) { RenderContext->SetVSync(VSync); }

    public: /*! Render bridge methods. */
        /*!*
         * Render context initialisation call function.
         * 
         * \param None.
         * \return None.
         */
        inline static void InitContext() { RenderContext->Init(); }

        /*!*
         * Render context deinitialisation call function.
         *
         * \param None.
         * \return None.
         */
        inline static void CloseContext() { RenderContext->Close(); }

        /*!*
         * Swap render targets function.
         * 
         * \param None.
         * \return None.
         */
        inline static void SwapBuffers()
        {
            RenderContext->SwapBuffers();
        }

        /*!*
         * Draw vertex array (vertex buffer and index buffer) to curent render target.
         * 
         * \param VertexArray - vertex array to draw verticces from.
         * \return None.
         */
        inline static void DrawIndices(const shared<vertex_array> &VertexArray)
        {
            RenderContext->DrawIndices(VertexArray);
        }

        /*!*
         * Instanced draw vertex array (vertex buffer and index buffer) to curent render target.
         * 
         * \param VertexArray - vertex array to draw verticces from.
         * \parma InstanceCount - instances of drawing vertex array count.
         * \return None.
         */
        inline static void DrawIndicesInstanced(const shared<vertex_array> &VertexArray, int InstanceCount)
        {
            RenderContext->DrawIndicesInstanced(VertexArray, InstanceCount);
        }

    public: /*! Sculpto library built-in backend API specific rendering objects getter function. */
        /*! Backend API specific single color material shader getter function. */
        inline static shared<shader_program> GetSingleColorMaterialShader()
        {
            return RenderContext->GetSingleColorMaterialShader();
        }

        /*! Backend API specific skybox material shader getter function. */
        inline static shared<shader_program> GetSkyboxMaterialShader()
        {
            return RenderContext->GetSkyboxMaterialShader();
        }

        /*! Backend API specific phong lighint model shader for geometry pass getter function. */
        inline static shared<shader_program> GetPhongGeometryShader()
        {
            return RenderContext->GetPhongGeometryShader();
        }

        /*! Backend API specific phong lighint model shader for lighting pass getter function. */
        inline static shared<shader_program> GetPhongLightingShader()
        {
            return RenderContext->GetPhongLightingShader();
        }

        /*! Backend API specific shadow pass shader getter function. */
        inline static shared<shader_program> GetShadowPassShader()
        {
            return RenderContext->GetShadowPassShader();
        }

        /*! Backend API specific full viewport mesh with tone mapping shader. */
        inline static shared<shader_program> GetToneMappingPassShader()
        {
            return RenderContext->GetToneMappingPassShader();
        }

        /*! Backend API specific full viewport mesh with tone mapping shader. */
        inline static shared<shader_program> GetGaussianBlurPassShader()
        {
            return RenderContext->GetGaussianBlurPassShader();
        }

        /*! Backend API specific gaussian blur pass shader getter function. */
        inline static shared<shader_program> GetTextureAddPassShader()
        {
            return RenderContext->GetTextureAddPassShader();
        }
    };
}
