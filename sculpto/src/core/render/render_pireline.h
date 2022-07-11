/*****************************************************************//**
 * \file   render_pireline.h
 * \brief  Render pipeline data storage class declaration module.
 *         Used by renderer to store submited data and cleared every frame.
 * 
 * \author Sabitov Kirill
 * \date   11 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    class mesh;
    class material;
    class frame_buffer;
    class constant_buffer;
    class shader_program;

    /* Scene rendering data. */
    struct pipeline_data
    {
        vec3       CameraPosition;     /* Submission camera direction vector. */
        float      Time;               /* Currently rendering frame time since porgram time. */
        vec3       CameraDirection;    /* Submission camer location vector. */
        int        ViewportWidth;      /* Currently rendering frame viewport width. */
        vec3       EnviromentAmbient;  /* Scene enviroment apbient color. */
        int        ViewportHeight;     /* Currently rendering frame viewport height. */
        u32        IsBloomActive;      /* Flag, showing wheather bloom effect is active or not. */
    };

    /* Render object render_pass_submission structure. */
    struct submission
    {
        shared<mesh>              Mesh;             /* Submitted to render mesh. */
        matr4                     Transform;        /* Submitted to render mesh tranformation matrix. */
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
        float __dummy;
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
    struct lights_storage
    {
        point_light PointLights[50] {};
        directional_light DirectionalLight {};
        spot_light SpotLights[50] {};
        u32 PointLightsCount {};
        u32 IsDirectionalLight {};
        u32 SpotLightsCount {};
    };

    /* Render pipeline data storage class. */
    struct render_pipeline
    {
        shared<shader_program>   ShadowPassShader {};    /* Shader program for filling depth buffer of shadow caster frme buffer. */
        shared<shader_program>   BloomApplyShader {};    /* Shader program for applying bloom effect (bluring bright colors) to main buffer. */
        shared<shader_program>   HDRApplyShader {};      /* Shader program for applying HDR effect to main frame buffer. */
        shared<frame_buffer>     BloomFrameBuffer {};    /* Frame buffer for apllying bloom effect. */
        shared<constant_buffer>  DataBuffer {};          /* Constant buffer for current scene data. */
        shared<constant_buffer>  LightsStorageBuffer {}; /* Constant buffer for current scene lights. */

        /* Every frame updating data. */
        shared<frame_buffer>     ShadowCasterFrameBuffer {}; /* Frame buffer (shadow map) of submited shadow caster with depth attachment only. */
        std::vector<submission>  SubmissionsList {};         /* Current scene list of submited to draw meshes. */
        pipeline_data            Data {};                    /* Current scene data. */
        lights_storage           LightsStorage {};           /* Current scene lights storage. */
        matr4                    ViewProjection {};          /* Current scene camera view projection matrix. */

        /**
         * Clear every frame updating pipeline data function.
         * 
         * \param None.
         * \return None.
         */
        void Clear()
        {
            ShadowCasterFrameBuffer.reset();
            SubmissionsList.clear();
            memset(&Data, 0, sizeof(pipeline_data));
            memset(&LightsStorage, 0, sizeof(lights_storage));
            memset(&ViewProjection, 0, sizeof(ViewProjection));
        }
    };
}
