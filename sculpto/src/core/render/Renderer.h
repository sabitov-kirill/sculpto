/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class definition module.
 *         Implements objects rendering functions via render_pass_submission queue.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "renderer_camera.h"

namespace scl
{
    /**
     * Renderer classes declaration.
     */
    class mesh;
    class material;
    class frame_buffer;
    class constant_buffer;

    struct point_light_component;
    struct directional_light_component;
    struct spot_light_component;

    /* Scene rendering data. */
    struct render_pass_data
    {
        vec3       CameraPosition {};     /* Submission camera direction vector. */
        float      Time {};               /* Currently rendering frame time since porgram time. */
        vec3       CameraDirection {};    /* Submission camer location vector. */
        int        ViewportWidth {};      /* Currently rendering frame viewport width. */
        int        ViewportHeight {};     /* Currently rendering frame viewport height. */
    };

    /* Render object render_pass_submission structure. */
    struct render_pass_submission
    {
        shared<mesh>              Mesh;             /* Submitted to render mesh. */
        shared<material>          Material;         /* Submitted to render mesh material. */
        matr4                     Transform;        /* Submitted to render mesh tranformation matrix. */

        /* Submission default constructor. */
        render_pass_submission(shared<mesh> Mesh, shared<material> Material, matr4 Transform) :
            Mesh(Mesh), Material(Material), Transform(Transform) {}
    };

    /* Point light structure. */
    struct point_light
    {
        vec3 Position;
        float Constant;
        vec3 Color;
        float Linear;
        float Quadratic;
        float __dummy[3];
    };

    /* Directional light structure. */
    struct directional_light
    {
        vec3 Direction;
        u32 IsShadows;
        vec3 Color;
        float __dummy2;
        matr4_data ViewProjection;
    };

    /* Spot light structure. */
    struct spot_light
    {
        vec3 Position;
        float InnerCutoffCos;
        vec3 Direction;
        float OuterCutoffCos;
        vec3 Color;
        float Epsilon;
    };

    /* Lights storage structure. */
    struct render_pass_lights_storage
    {
        point_light PointLights[50] {};
        directional_light DirectionalLight {};
        spot_light SpotLights[50] {};
        u32 PointLightsCount {};
        u32 IsDirectionalLight {};
        u32 SpotLightsCount {};
    };

    /* Renderer class. */
    class renderer
    {
    private: /* renderer data. */
        static std::vector<render_pass_submission> SubmissionQueue;  /* Render pass meshes submissions queue.
                                                                      * Each render_pass_submission queue contains submitted mesh and data for its rendering.
                                                                      * Submission queue could be rendered and cleared via flush function.
                                                                      */

        static shared<constant_buffer>     PipelineDataBuffer;                 /* Constant buffer for current scene data. */
        static render_pass_data            PipelineData;                       /* Current scene data. */
        static matr4                       PipelineViewProjection;             /* Current scene camera view projection matrix. */
        static shared<constant_buffer>     PipelineLightsStorageBuffer;        /* Constant buffer for current scene lights. */
        static render_pass_lights_storage  PipelineLightsStorage;              /* Current scene lights storage. */
        static shared<material>            PipelineShadowCasterMaterial;       /* Current scene shadow caster material with shader for shadow pass. */
        static shared<frame_buffer>        PipelineShadowCasterFrameBuffer;    /* Current scene shadow caster frame buffer with shadow map texture. */

    private:
        /**
         * Draw mesh function.
         * 
         * \param Mesh - mesh to draw.
         * \param Material - mesh material.
         * \param Transform - mesh tranformations matrix.
         * \return None.
         */
        static void Draw(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform);

        /**
         * Draw mesh during shadow pass function.
         * 
         * \param Mesh - mesh to draw.
         * \param Transform - mesh transformation matrix.
         * \return None.
         */
        static void DrawShadowPass(const shared<mesh> &Mesh, const matr4 &Transform);

    public:
        /**
         * Begin render pass function.
         * Updates scene data buffer.
         * 
         * \param ViewProjection - scene camera view projection matrix.
         * \param CameraDirection - scene camera direction vector.
         * \param CameraPosition - scene camer location.
         * \param ViewportWidth - scene viewport width.
         * \param ViewportHeight - scene viewport height.
         * \return None.
         */
        static void StartPipeline(const matr4 &ViewProjection,
                                  const vec3 &CameraDirection, const vec3 &CameraPosition,
                                  const int ViewportWidth, const int ViewportHeight);

        /**
         * End render pass function.
         * Flush render_pass_submission quque and draw all meshes to specified frame buffer function.
         *
         * \param FrameBuffer - frame buffer to draw in.
         * \return None.
         */
        static void EndPipeline(const shared<frame_buffer> &FrameBuffer = nullptr);

        /**
         * Submit point light function.
         * 
         * \param Position - light possition.
         * \param Color - light color.
         * \param Constant - light attenutation constant coefficient.
         * \param Linear - light attenuation liear coefficient.
         * \param Quadratic - light attenuation quadratic coefficient.
         * \return None.
         */
        static void SubmitPointLight(const vec3 &Position, const vec3 &Color, float Constant, float Linear, float Quadratic);

        /**
         * Submit point light function.
         *
         * \param Direction - light direction.
         * \param Color - light color.
         * \param IsShadows - is directional light casts shadows flag.
         * \param VP - matrix for light cooredinate space tranformation. Need only for shadow casting.
         * \param FrameBuffer - frame buffer for shadow pass.
         * \return None.
         */
        static void SubmitDirectionalLight(const vec3 &Direction, const vec3 &Color, bool IsShadows = false,
                                           const matr4 &ViewProjection = {}, shared<frame_buffer> FrameBuffer = nullptr);

        /**
         * Submit point light function.
         *
         * \param Position - light possition.
         * \param Direction - light direction
         * \param Color - light color.
         * \param InnerCutoffCos - cosine of inner spot light cone angle.
         * \param OuterCutoffCos - cosine of outer spot light cone angle.
         * \param Epsilon - (OuterCutoffCos - InnerCutoffCos), for optimization.
         */
        static void SubmitSpotLight(const vec3 &Position, const vec3 &Direction, const vec3 &Color, float InnerCutoffCos, float OuterCutoffCos, float Epsilon);

        /**
         * Calculate mesh transorm matrix and submit to queue (rendered while flush call) function.
         *
         * \param Mesh - mesh to submit to queue.
         * \param Material - mesh material.
         * \param Scale - mesh scale factor along 3 axies.
         * \param Angles - mesh rotation angles factor along 3 axies.
         * \param Position - mesh world position.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const shared<material> &Material, 
                           const vec3 &Scale, const vec3 &Angles, const vec3 &Position);

        /**
         * Submit mesh to render_pass_submission queue (rendered while flush call) function.
         * 
         * \param Mesh - mesh to submit to queue.
         * \param Material - mesh material.
         * \param Transform - mesh tranformations matrix.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform);
    };
}
