/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class definition module.
 *         Implements objects rendering functions via render_pass_submission queue.
 *
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "render_pipeline.h"
#include "core/resources/camera.h"

namespace scl
{
    /* Classes declaration. */
    class texture_2d;

    /* Renderer class. */
    class renderer
    {
    private: /* Renderer data. */
        static render_pipeline Pipeline;

    public: /* Helper functions. */
        /**
         * Draw mesh geometry (for geometry pass) function.
         *
         * \param Mesh - mesh to draw.
         * \param Transform - mesh tranformations matrix.
         * \return None.
         */
        static void DrawGeometry(const shared<mesh> &Mesh, const matr4 &Transform);

        /**
         * Draw mesh depth only (for shadow pass, creating shadow mpa of shadow caster) function.
         *
         * \param Mesh - mesh to draw.
         * \param Transform - mesh transformation matrix.
         * \return None.
         */
        static void DrawDepth(const shared<mesh> &Mesh, const matr4 &Transform);

        /**
         * Draw fullscreen quad to call binded shader for each pixel of frame buffer.
         *
         * \param None.
         * \return None.
         */
        static void DrawFullscreenQuad();

    private:
        /**
         * Add texture colors to main color attachment of detination frame buffer function.
         *
         * \param Destination - destination frame buffer to add texture color in.
         * \param Texture - texture to add to destination frame buffer.
         * \return None.
         */
        static void ApplyTexture(const shared<frame_buffer> &Destination, const shared<texture_2d> &SourceTexture);

        /**
         * Add texture colors, blurred with two-pass gaussian blur, to main color attachment of destination frame buffer function.
         *
         * \param Destination - destination frame buffer.
         * \param Source - source texture to blur and apply to destination frame buffer.
         * \param Iterations - gaussian blur iterations count.
         * \return None.
         */
        static void ApplyBluredTexture(const shared<frame_buffer> &Destination, const shared<texture_2d> &Source, int Iterations);

    private: /* Render passes computation functions. */
        /**
         * Apply depth computation of current scene (from pipeline submissions list) to destination buffer.
         * 
         * \param None.
         * \return None.
         */
        static void ComputeDepth();

        /**
         * Apply geometry computation of current scene (from pipeline submissions list) to pipeline geometry buffer (concrete buffer depends on current lighting model).
         *
         * \param None.
         * \return None.
         */
        static void ComputeGeometry();

        /**
         * Apply default lighting pass (of selected lighting model) to frame buffer (concrete buffer depends on pipeline settings - HDR on/off).
         * 
         * \param None.
         * \return None.
         */
        static void ComputateLighting();

        /**
         * Apply bloom effect to main color attachment of detination frame buffer function.
         *
         * \param Destination - destination frame buffer.
         * \param BrightColorsTexture- texture with bright colors of blooming image.
         * \return None.
         */
        static void ComputeBloom();

        /**
         * Apply tone mapping algorithm, translating HDR to main frame buffer.
         *
         * \param None.
         * \return None.
         */
        static void ComputeToneMapping();

    public: /* Renderer API functions. */
        /**
         * Renderer initialization function.
         * 
         * \param None.
         * \return None.
         */
        static void Initialize();

        /**
         * Begin render pass function.
         * Updates renderer pipeline data buffer.
         *
         * \param Camera - camera to render in.
         * \param EnviromantAmbient - rendering scene enviroment ambient color.
         * \return None.
         */
        static void StartPipeline(const camera &Camera, const vec3 &EnviromentAmbiente);

        /**
         * End render pass function.
         * Flush render_pass_submission quque and draw all meshes to specified frame buffer function.
         *
         * \param Destination - destination frame buffer to draw in.
         * \return None.
         */
        static void EndPipeline();

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
         * \param ViewProjection - shadow caster view projection matrix.
         * \param ShadowMap - shadow caster shadow map frame buffer.
         * \return None.
         */
        static void SubmitDirectionalLight(const vec3 &Direction, const vec3 &Color, bool IsShadows = false,
                                           const matr4 &ViewProjection = {}, const shared<frame_buffer> &ShadowMap = nullptr);

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
         * \param Scale - mesh scale factor along 3 axies.
         * \param Angles - mesh rotation angles factor along 3 axies.
         * \param Position - mesh world position.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const vec3 &Scale, const vec3 &Angles, const vec3 &Position);

        /**
         * Submit mesh to render_pass_submission queue (rendered while flush call) function.
         *
         * \param Mesh - mesh to submit to queue.
         * \param Transform - mesh tranformations matrix.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const matr4 &Transform);
    };
}
