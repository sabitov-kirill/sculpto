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
    class shader_program;
    class mesh;
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
        MATR3,
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

        /* Rendering context shader constants. */
        static const int BINDING_POINT_SCENE_DATA             = 0;
        static const int BINDING_POINT_MATERIAL_DATA          = 5;
        static const int BINDING_POINT_LIGHTS_STORAGE         = 10;
        static const int BINDING_POINT_SHADOW_CASTERS_STORAGE = 11;
        static const int BINDING_POINT_FREE                   = 20;
        static const int TEXTURE_SLOT_MATERIAL_DIFFUSE        = 0;
        static const int TEXTURE_SLOT_MATERIAL_SPECULAR       = 1;
        static const int TEXTURE_SLOT_MATERIAL_EMISSION_MAP   = 2;
        static const int TEXTURE_SLOT_MATERIAL_NORMAL_MAP     = 3;
        static const int TEXTURE_SLOT_SHADOW_MAP              = 5;
        static const int TEXTURE_SLOT_HDR_BUFFER              = 6;
        static const int TEXTURE_SLOT_BLUR_BUFFER             = 7;
        static const int LIGHTS_MAX_POINT                     = 50;
        static const int LIGHTS_MAX_SPOT                      = 50;

        static const int MESH_RESTART_INDEX = -1;

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

    public: /* Sculpto library built-in backend API specific rendering objects getter function. */
        /* Backend API specific single color material shader getter function. */
        virtual shared<shader_program> GetSingleColorMaterialShader() const = 0;

        /* Backend API specific phong lighting model material shader getter function. */
        virtual shared<shader_program> GetPhongMaterialShader() const = 0;

        /* Backend API specific shadow pass shader getter function. */
        virtual shared<shader_program> GetShadowPassShader() const = 0;

        /* Backend API specific tone mapping pass shader getter function. */
        virtual shared<shader_program> GetToneMappingPassShader() const = 0;

        /* Backend API specific gaussian blur pass shader getter function. */
        virtual shared<shader_program> GetGaussianBlurPassShader() const = 0;

        /**
         * Rendering context creation function.
         *
         * \param None.
         * \return pointer to created rendering context.
         */
        static unique<render_context> Create();
    };
}
