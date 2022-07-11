/*****************************************************************//**
 * \file   scene.h
 * \brief  Application scene sustem class defintion module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include <entt.hpp>
#include "core/components/camera_controller_component.h"

namespace scl
{
    /* Classes declaration. */
    class event;
    class scene_object;
    class frame_buffer;
    class shader;
    class mesh;

    /* Application scene class. */
    class scene
    {
        friend class scene_object;
        friend class scene_hierarchy_window;

    private: /* Application scene data. */
        entt::registry Registry {};                 /* Container for all components.
                                                     * Contains both enitty datas and Ids.
                                                     */

        camera_controller_component *MainCamera {};       /* Scene main camera controller. */
        int                  ViewportWidth { 16 };        /* Scene window viewport width. */
        int                  ViewportHeight { 16 };       /* Scene window viewport height. */
        bool                 RenderToSwapChainBuffer {};  /* Flag, showing whether render to swap chain buffer or not. */
        bool                 IsHDR { true };              /* Flag, showing wheather use hight dynamic range for colors while rendering frame. */
        shared<mesh>         HDRPassFullscreenQuad {};    /* Full screen quad mesh for HDR Tone mapping pass. */
        bool                 IsBloom { true };            /* Flag, showing wheather apply bloom effect or not while rendering frame. */
        shared<mesh>         BloomBluringFullscreenQuad {};

        float UpdateDelay {};  /* Scripts update call timer. */

    public:
        shared<frame_buffer> BloomBlurringBuffers[2] {};  /* Frame buffers for blurring bright colors during bloom effect apply. */
        vec3    EnviromentAmbient { 0.1f };     /* Enviroment ambient color. */
        float   Exposure { 1 };                 /* Tone mapping exposure level. Works only if rendering with HDR. */

    public: /* Application scene getter/setter functions. */
        /* Scene main camera getter function */
        const camera_controller_component &GetMainCamera() const { return *MainCamera; }
        /* Scene window viewport width getter function. */
        int GetViewportWidth() const { return ViewportWidth; }
        /* Scene window viewport height getter function. */
        int GetViewportHeight() const { return ViewportHeight; }
        /* Flag, showing wheather use hight dynamic range for colors while rendering frame getter function. */
        bool GetIsHDR() const { return IsHDR; }
        /* Flag, showing wheather apply bloom effect or not while rendering frame getter function. */
        bool GetIsBloom() const { return IsBloom; }

        /**
         * Set scene main camera by name function.
         *
         * \param Name - camera to set as main name.
         * \return None.
         */
        void SetMainCamera(const std::string &Name);

        /**
         * Set flag, indicating whether render to swap chain buffer or not.
         *
         * \param RenderToSwapChainBuffer - flag, showing whether render to swap chain buffer or not.
         * \return None.
         */
        void SetRenderToSwapChainBuffer(bool RenderToSwapChainBuffer);

        /**
         * Set flag, indicating wheather use high dynamic range of colors while rendering.
         * 
         * \param IsHDR - flag, showing wheather use HDR, or not.
         * \return None.
         */
        void SetIsHDR(bool IsHDR);

        /**
         * Set flag, indicatinf whaether apply bloom effect while rendering frame or not.
         * 
         * \param IsBloom - flag, indicatinf whaether apply bloom effect while rendering frame or not.
         * \return None.
         */
        void SetIsBloom(bool IsBloom);

    public: /* Application scene methods. */
        /* Scene default constructor. */
        scene();

        /* Scene default destructor. */
        ~scene();

        /**
         * Scene update callback function.
         * 
         * \param DeltaTime
         * \return None.
         */
        void OnUpdate(float DeltaTime);

        /**
         * Scene on event callback function.
         * 
         * \param Event
         * \return None.
         */
        void OnEvent(event &Event);

        /**
         * Create scene object function.
         * 
         * \param Name - object name (mainly for debug purpose).
         * \return created object.
         */
        scene_object CreateObject(const std::string &Name = "");

    private: /* Application scene methods. */
        void Render();
        void CallUpdate();
    };
}
