/*!****************************************************************//*!*
 * \file   render_pireline.h
 * \brief  Render pipeline data storage class declaration module.
 *         Used by renderer to store submited data and cleared every frame.
 * 
 * \author Sabitov Kirill
 * \date   11 July 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "core/render/render_bridge.h"
#include "core/render/primitives/buffer.h"
#include "utilities/assets_manager/shaders_load.h"

namespace scl
{
    /*! Classes declaration. */
    class mesh;
    class material;
    class frame_buffer;
    class constant_buffer;
    class shader_program;

    /*! Scene rendering data. */
    struct pipeline_data
    {
        vec3  CameraPosition;       /*! Submission camera direction vector. */
        float Time;                 /*! Currently rendering frame time since porgram time. */
        vec3  CameraDirection;      /*! Submission camer location vector. */
        int   ViewportWidth;        /*! Currently rendering frame viewport width. */
        vec3  CameraUpDirection;    /*! Submission camer location vector. */
        int   ViewportHeight;       /*! Currently rendering frame viewport height. */
        vec3  CameraRightDirection; /*! Submission camer location vector. */
        bool  IsHDR;                /*! Flag, showing wheather HDR frame buffer, tone mapped to destination frame buffer is active or not. */
        vec3  EnviromentAmbient;    /*! Scene enviroment apbient color. */
        float Exposure;             /*! Exposure level for exposure tone mapping algoritm (applyed only if HDR is active). */
        bool  IsBloom;              /*! Flag, showing wheather bloom effect is active or not. */
        int   BloomAmount;          /*! Iteration of blur while applying bloom effect. */
    };

    /*! Render object render_pass_submission structure. */
    struct submission
    {
        shared<mesh> Mesh;      /*! Submitted to render mesh. */
        matr4        Transform; /*! Submitted to render mesh tranformation matrix. */
    };

    /*! Point light structure. */
    struct point_light
    {
        vec3 Position;
        float Constant;
        vec3 Color;
        float Linear;
        float Quadratic;
        float __dummy[3];
    };

    /*! Directional light structure. */
    struct directional_light
    {
        vec3 Direction;
        u32 IsShadows;
        vec3 Color;
        float __dummy;
        matr4_data ViewProjection;
    };

    /*! Spot light structure. */
    struct spot_light
    {
        vec3 Position;
        float InnerCutoffCos;
        vec3 Direction;
        float OuterCutoffCos;
        vec3 Color;
        float Epsilon;
    };

    /*! Lights storage structure. */
    struct lights_storage
    {
        point_light PointLights[50] {};
        directional_light DirectionalLight {};
        spot_light SpotLights[50] {};
        u32 PointLightsCount {};
        u32 IsDirectionalLight {};
        u32 SpotLightsCount {};
    };

    /*! Render pipeline data storage class. */
    struct render_pipeline
    {
        bool IsInitialized { false };

        /*! Render resources used by renderer. */
        shared<shader_program>  ShadowPassShader {};         /*! Shader program for filling depth buffer of shadow caster frame buffer. */
        shared<shader_program>  PhongLightingApplyShader {}; /*! Shader program for applying deffered phong lighting. */
        shared<shader_program>  GaussianBlurApplyShader {};  /*! Shader program for applying bloom effect (bluring bright colors) to main buffer. */
        shared<shader_program>  TextureAddShader {};         /*! Shader program for combining (additive bluring) two textuers. */
        shared<shader_program>  ToneMappingApplyShader {};   /*! Shader program for applying HDR effect to main frame buffer. */
        shared<constant_buffer> DataBuffer {};               /*! Constant buffer for pipeline data. */
        shared<constant_buffer> LightsStorageBuffer {};      /*! Constant buffer for pipeline lights storage. */

        /*! Pipeline shadow map frame buffer. */
        shared<frame_buffer> ShadowMap {};

        /*! Pipeline camera frame buffers to render in. */
        shared<frame_buffer> MainFrameBuffer {};     /*! Main renderer frame buffer. */
        shared<frame_buffer> GBuffer {};             /*! Frame buffer for geometry pass. */
        shared<frame_buffer> HDRFrameBuffer {};      /*! HDR Frame buffer, then tone mapped to destination frame buffer. */
        shared<frame_buffer> BlurFrameBuffers0 {};   /*! Frame buffer for apllying gaussian blur effect. */
        shared<frame_buffer> BlurFrameBuffers1 {};   /*! Frame buffer for apllying gaussian blur effect. */

        /*! Pipeline camera view projection matrix. */
        matr4 ViewProjection;

        /*! Every frame updating data. */
        std::vector<submission> SubmissionsList {}; /*! Pipeline list of submited to draw meshes. */
        lights_storage          LightsStorage {};   /*! Pipeline lights storage. */
        pipeline_data           Data {};            /*! Pipeline data. */

        /*! Render pipeline deault constructor. */
        render_pipeline() = default;

        /*! Render pipeline default destructor. */
        ~render_pipeline() = default;

        /*!*
         * Render pipeline initializetion function..
         * 
         * \param None.
         * \return None.
         */
        void Initalize()
        {
            IsInitialized = true;

            ShadowPassShader         = render_bridge::GetShadowPassShader();
            PhongLightingApplyShader = render_bridge::GetPhongLightingShader();
            GaussianBlurApplyShader  = render_bridge::GetGaussianBlurPassShader();
            TextureAddShader         = render_bridge::GetTextureAddPassShader();
            ToneMappingApplyShader   = render_bridge::GetToneMappingPassShader();
            DataBuffer               = constant_buffer::Create(sizeof(pipeline_data));
            LightsStorageBuffer      = constant_buffer::Create(sizeof(lights_storage));
        }

        /*!*
         * Clear every frame updating pipeline data function.
         * 
         * \param None.
         * \return None.
         */
        void Clear()
        {
            SubmissionsList.clear();
            std::memset(&Data, 0, sizeof(pipeline_data));
            std::memset(&LightsStorage, 0, sizeof(lights_storage));

            ShadowMap.reset();
            MainFrameBuffer.reset();
            GBuffer.reset();
            HDRFrameBuffer.reset();
            BlurFrameBuffers0.reset();
            BlurFrameBuffers1.reset();
        }
    };
}
