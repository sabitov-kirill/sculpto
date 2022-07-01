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
#include "primitives/mesh.h"

namespace scl
{
    /* Render bridge class. */
    class render_bridge
    {
    private: /* Render bridge data. */
        static shared<render_context> RenderContext;

    public:
        static void Clear();
        static void SwapBuffers();
        static void DrawIndices(const mesh &VertexArray);
    };
}
