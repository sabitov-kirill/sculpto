/*****************************************************************//**
 * \file   render_context.h
 * \brief  Base, abstract, backend render api independent render
 *         context class definition module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Vertex array class declaration. */
    class vertex_array;
    enum class mesh_type;

    /* Shader variable type enum. */
    enum class shader_variable_type: u8
    {
        BOOL,
        INT,
        INT2,
        INT3,
        INT4,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        MATR4,
    };

    /**
     * Get size of specified shader variable type fucntion.
     *
     * \param Type - shader variable type to get size of.
     * \return size of specified type.
     */
    u32 GetShaderVariableTypeSize(shader_variable_type Type);

    /**
     * Get components count of specified shader variable type fucntion.
     *
     * \param Type - shader variable type to get components count of.
     * \return components count of specified type.
     */
    u32 GetShaderVariableComponentsCount(shader_variable_type Type);

    /* Base abstract render context class. */
    class render_context
    {
    public: /* Rendering context data. */
        enum class api
        {
            OpenGL,
            DirectX
        };

        /* Rendering context backend API getter function. */
        static api GetApi() { return Api; }

        /* Rendering context shader global binding points. */
        static const int MESH_DATA_BINDING_POINT = 0;
        static const int MATERIAL_DATA_BINDING_POINT = 1;
        static const int MATERIAL_SURFACE_TEXTURE_SLOT = 0;
        static const int MATERIAL_NORMAL_MAP_TEXTURE_SLOT = 1;
        static const int MATERIAL_ALBEDO_MAP_TEXTURE_SLOT = 2;

    private: /* render context data. */
        static api Api; /* Render system backend api. */

    public: /* Render context methods. */
        /* Default render context constructor. */
        render_context() {};

        /* Default render context destructor. */
        virtual ~render_context() {};

        /**
         * Render context initialisation function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Init() = 0;

        /**
         * Render context deinitialisation function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Close() = 0;

        /**
         * Frame clear color setter function.
         *
         * \param Color - new frame clear color.
         * \return None.
         */
        virtual void SetClearColor(const vec4 &Color) = 0;

        /**
         * Setting render wire frame mode function.
         *
         * \param IsWireframe - wireframe rendering flag.
         * \return None.
         */
        virtual void SetWireframeMode(bool IsWireframe) = 0;

        /**
         * Clear current render target function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Clear() = 0;

        /**
         * Swap frame buffers function.
         *
         * \param None.
         * \return None.
         */
        virtual void SwapBuffers() = 0;

        /**
         * Conetxt resize function.
         *
         * \param Width, Height - new width and hight of applicatino window.
         * \return None.
         */
        virtual void Resize(int Width, int Height) = 0;

        /**
         * Draw vertices function.
         * 
         * \param Mesh - mesh, containing vertices and vertex indices to draw.
         * \return None.
         */
        virtual void DrawIndices(const shared<vertex_array> &Mesh) = 0;

        /**
         * Draw vertices instanced function.
         *
         * \param Mesh - mesh, containing vertices and vertex indices to draw.
         * \return None.
         */
        virtual void DrawIndicesInstanced(const shared<vertex_array> &Mesh, int InstanceCount) = 0;

        /**
         * Rendering context creation function.
         *
         * \param None.
         * \return pointer to created rendering context.
         */
        static unique<render_context> Create();
    };
}
