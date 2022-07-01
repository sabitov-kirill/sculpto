/*****************************************************************//**
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
    /* Vertex buffer and arary classes declaration. */
    class vertex_buffer;
    class vertex_array;

    /* Render bridge class. */
    class render_bridge
    {
    private: /* Render bridge data. */
        static shared<render_context> RenderContext;

    public:
        /**
         * Render context initialisation call function.
         * 
         * \param None.
         * \return None.
         */
        inline static void InitContext() { RenderContext->Init(); }

        /**
         * Render context deinitialisation call function.
         *
         * \param None.
         * \return None.
         */
        inline static void CloseContext() { RenderContext->Close(); }

        /**
         * Render context resize default render target call function.
         *
         * \param None.
         * \return None.
         */
        inline static void ResizeContext(int Width, int Height)
        {
            RenderContext->Resize(Width, Height);
        }

        /**
         * Render context set clear color call function.
         * 
         * \param ClearColor - new clear color.
         * \return None.
         */
        inline static void SetClearColor(const vec4 &ClearColor)
        {
            RenderContext->SetClearColor(ClearColor);
        }

        /**
         * Render context set wire frame mode call function.
         * 
         * \param IsWireframe - is wire frame mode enabled flag.
         * \return None.
         */
        inline static void SetWireframeMode(bool IsWireframe)
        {
            RenderContext->SetWireframeMode(IsWireframe);
        }

        /**
         * Cleare current render target function.
         * 
         * \param None.
         * \return None.
         */
        inline static void Clear() { RenderContext->Clear(); }

        /**
         * Swap render targets function.
         * 
         * \param None.
         * \return None.
         */
        inline static void SwapBuffers()
        {
            RenderContext->SwapBuffers();
        }

        /**
         * Draw vertex array (vertex buffer and index buffer) to curent render target.
         * 
         * \param VertexArray - vertex array to draw verticces from.
         * \return None.
         */
        inline static void DrawIndices(const shared<vertex_array> &VertexArray)
        {
            RenderContext->DrawIndices(VertexArray);
        }

        /**
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
    };
}
