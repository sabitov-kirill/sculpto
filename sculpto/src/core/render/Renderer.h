/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class definition module.
 *         Implements objects rendering functions via submission queue.
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

    struct submission_data
    {
        matr4  ViewProjection {};  /* Submission camer view projection matrix. */
        vec3   CameraDirection {}; /* Submission camera direction vector. */
        vec3   CameraLocation {};  /* Submission camer location vector. */
        float Time;                /* Currently rendering frame time since porgram time. */
        int ViewportWidth {};      /* Currently rendering frame viewport width. */
        int ViewportHeight {};     /* Currently rendering frame viewport height. */
    };

    /* Render submission structure. Contains current scene camera, enviroment etc. */
    struct submission
    {
        shared<mesh>              Mesh;             /* Submitted to render mesh. */
        shared<material>          Material;         /* Submitted to render mesh material. */
        matr4                     Transform;        /* Submitted to render mesh tranformation matrix. */
        const renderer_camera    &Camera;           /* Scene camera, active during submission call. */

        /* Submission default constructor. */
        submission(shared<mesh> Mesh, shared<material> Material, matr4 Transform, const renderer_camera &Camera) :
            Mesh(Mesh), Camera(Camera), Material(Material), Transform(Transform) {}
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
        float __dummy1;
        vec3 Color;
        float __dummy2;
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
    struct lights_storage
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
        static std::vector<submission> SubmissionQueue;  /* Render submission queue.
                                                          * Each submission queue contains submitted mesh and data for its rendering.
                                                          * Submission queue could be rendered and cleared via flush function.
                                                          */

        const static renderer_camera *SceneCamera;       /* Current scene camera. Added to subbmision object while Submit call. */

        static shared<constant_buffer> SceneLightsStorageBuffer; /* Constant buffer for current scene lights. */
        static lights_storage SceneLightsStorage;                /* Current scene lights storage. */

    private:
        /**
         * Draw mesh function.
         * 
         * \param Mesh - mesh to draw.
         * \param Material - mesh material.
         * \param Transform - mesh tranformations matrix.
         * \param Camera - camera to take view and projection matrix from.
         * \return None.
         */
        static void Draw(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform, const renderer_camera &Camera);

    public:
        /**
         * Begin scene function.
         * Runs computations for concrete scene,
         * memoize camera & enviroment data for rendering
         *
         * \param Camera - scene camera to use while rendering scene.
         * \return None.
         */
        static void SubmitCamera(const renderer_camera &Camera);

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
         * \return None.
         */
        static void SubmitDirectionalLight(const vec3 &Direction, const vec3 &Color);

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
         * Submit mesh to submission queue (rendered while flush call) function.
         * 
         * \param Mesh - mesh to submit to queue.
         * \param Material - mesh material.
         * \param Transform - mesh tranformations matrix.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform);

        /**
         * Flush submission quque and draw all meshes to specified frame buffer function.
         *
         * \param FrameBuffer - frame buffer to draw in.
         * \return None.
         */
        static void Flush(const shared<frame_buffer> &FrameBuffer);

        /**
         * Flush submission quque and draw all meshes to default screen frame buffer function.
         * 
         * \param None.
         * \return None.
         */
        static void FlushToDefaultFrameBuffer();
    };
}
