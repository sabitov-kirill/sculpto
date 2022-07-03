/*****************************************************************//**
 * \file   scene.h
 * \brief  Application scene sustem class defintion module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include <entt.hpp>
#include "../components/camera_controller_component.h"

namespace scl
{
    /* Event class declaration. */
    class event;

    /* Application scene class. */
    class scene
    {
        friend class scene_object;

    private: /* Application scene data. */
        entt::registry Registry;                    /* Container for all components.
                                                     * Contains both enitty datas and Ids.
                                                     */
        
        float UpdateDelay {};                       /* Scripts update call delay timer. */

        camera_controller_component *MainCamera {};        /* Scene main camera controller. */
        float ViewportWidth { 16 }, ViewportHeight { 16 }; /* Scene window viewport size. */
        bool RenderToSwapChainBuffer;                      /* Flag, indicating whether render to swap chain buffer or not. */

    public: /* Application scene getter/setter functions. */
        /* Scene main camera getter function */
        const camera_controller_component &GetMainCamera() const { return *MainCamera; }
        /* Scene window viewport width getter function. */
        float GetViewportWidth() const { return ViewportWidth; }
        /* Scene window viewport height getter function. */
        float GetViewportHeight() const { return ViewportHeight; }

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
         * \param RenderToSwapChainBuffer - flag, indicating whether render to swap chain buffer or not.
         * \return None.
         */
        void SetRenderToSwapChainBuffer(bool RenderToSwapChainBuffer);

    private: /* Application scene methods. */
        void Render();
        void CallUpdate();

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
    };
}
