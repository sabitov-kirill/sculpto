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

    /* Render contextu culling mode. */
    enum class render_cull_face_mode
    {
        OFF,
        BACK,
        FRONT
    };

    /* Render context backend api enum. */
    enum class render_context_api
    {
        OpenGL,
        DirectX
    };

    /* Base abstract render context class. */
    class render_context
    {
    private: /* Render context data. */
        static render_context_api Api; /* Render system backend api. */

    public: /* Render context data getter/setter functions. */
        /* Rendering context backend API getter function. */
        static render_context_api GetApi() { return Api; }

        /* Frame clear color setter function. */
        virtual const vec4 &GetClearColor() const = 0;
        /* Render wire frame mode setter function. */
        virtual bool GetWireframeMode() const = 0;
        /* Render culling mode setter function. */
        virtual render_cull_face_mode GetCullingMode() const = 0;
        /* Render virtual syncronisation flag getter function. */
        virtual bool GetVSync() const = 0;

        /* Frame clear color setter function. */
        virtual void SetClearColor(const vec4 &ClearColor) = 0;
        /* Render wire frame mode setter function. */
        virtual void SetWireframeMode(bool IsWireframe) = 0;
        /* Render culling mode setter function. */
        virtual void SetCullingMode(render_cull_face_mode CullingMode) = 0;
        /* Render virtual syncronisation flag setter function. */
        virtual void SetVSync(bool VSync) = 0;

    public:
        /**
         * Get size of specified shader variable type fucntion.
         *
         * \param Type - shader variable type to get size of.
         * \return size of specified type.
         */
        static u32 GetShaderVariableTypeSize(shader_variable_type Type);

        /**
         * Get components count of specified shader variable type fucntion.
         *
         * \param Type - shader variable type to get components count of.
         * \return components count of specified type.
         */
        static u32 GetShaderVariableComponentsCount(shader_variable_type Type);

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
         * Swap frame buffers function.
         *
         * \param None.
         * \return None.
         */
        virtual void SwapBuffers() = 0;

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

    public: /* Sculpto library built-in backend API specific rendering objects getter function. */
        /* Backend API specific single color material shader getter function. */
        virtual shared<shader_program> GetSingleColorMaterialShader() const = 0;
        /* Backend API specific phong lighint model shader for geometry pass getter function. */
        virtual shared<shader_program> GetPhongGeometryShader() const = 0;
        /* Backend API specific phong lighint model shader for lighting pass getter function. */
        virtual shared<shader_program> GetPhongLightingShader() const = 0;
        /* Backend API specific shadow pass shader getter function. */
        virtual shared<shader_program> GetShadowPassShader() const = 0;
        /* Backend API specific tone mapping pass shader getter function. */
        virtual shared<shader_program> GetToneMappingPassShader() const = 0;
        /* Backend API specific gaussian blur pass shader getter function. */
        virtual shared<shader_program> GetGaussianBlurPassShader() const = 0;
        /* Backend API specific gaussian blur pass shader getter function. */
        virtual shared<shader_program> GetTextureAddPassShader() const = 0;

    public: /* Rendering context shader constants. */
        static const int BINDING_POINT_SCENE_DATA              = 0;
        static const int BINDING_POINT_MATERIAL_DATA           = 5;
        static const int BINDING_POINT_LIGHTS_STORAGE          = 10;
        static const int BINDING_POINT_SHADOW_CASTERS_STORAGE  = 11;
        static const int BINDING_POINT_FREE                    = 20;

        static const int TEXTURE_SLOT_MATERIAL_DIFFUSE         = 0;
        static const int TEXTURE_SLOT_MATERIAL_SPECULAR        = 1;
        static const int TEXTURE_SLOT_MATERIAL_EMISSION_MAP    = 2;
        static const int TEXTURE_SLOT_MATERIAL_NORMAL_MAP      = 3;
        static const int TEXTURE_SLOT_SHADOW_MAP               = 5;
        static const int TEXTURE_SLOT_HDR_BUFFER               = 6;
        static const int TEXTURE_SLOT_BLUR_BUFFER              = 7;
        static const int TEXTURE_SLOT_APPLY_SOURCE             = 9;
        static const int TEXTURE_SLOT_APPLY_TEXTURE_ADD        = 10;
        static const int TEXTURE_SLOT_GEOM_PASS_OUT_POSITION             = 11;
        static const int TEXTURE_SLOT_GEOM_PASS_OUT_NORMAL               = 12;
        static const int TEXTURE_SLOT_GEOM_PASS_OUT_COLOR                = 13;
        static const int TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_DIFFUSE        = 14;
        static const int TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_SPECULAR       = 15;
        static const int TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_SHININESS      = 16;
        static const int TEXTURE_SLOT_LIGHTING_PASS_OUT_COLOR            = 17;
        static const int TEXTURE_SLOT_LIGHTING_PASS_OUT_BRIGHT_COLOR     = 18;

        static const int LIGHTS_MAX_POINT                      = 50;
        static const int LIGHTS_MAX_SPOT                       = 50;

        static const int COLOR_ATTACHMENT_GEOM_PASS_OUT_POSITION             = 0;
        static const int COLOR_ATTACHMENT_GEOM_PASS_OUT_NORMAL               = 1;
        static const int COLOR_ATTACHMENT_GEOM_PASS_OUT_COLOR                = 2;
        static const int COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_DIFFUSE        = 3;
        static const int COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SPECULAR       = 4;
        static const int COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SHININESS      = 5;
        static const int COLOR_ATTACHMENT_LIGHTING_PASS_OUT_COLOR            = 0;
        static const int COLOR_ATTACHMENT_LIGHTING_PASS_OUT_BRIGHT_COLOR     = 1;

        static const int MESH_RESTART_INDEX = -1;
    };
}
