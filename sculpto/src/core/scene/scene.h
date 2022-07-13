/*****************************************************************//**
 * \file   scene.h
 * \brief  Application scene sustem class defintion module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include <entt.hpp>
#include "core/components/camera_component.h"

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
        entt::registry Registry {}; /* Container for all components.
                                     * Contains both enitty datas and Ids.
                                     */

        camera_component *MainCamera {};   /* Scene main camera controller. */
        int   ViewportWidth { 16 };        /* Scene viewport width. */
        int   ViewportHeight { 16 };       /* Scene viewport height. */
        float UpdateDelay {};              /* Scene scripts update call timer. */
        vec3  EnviromentAmbient { 0.1f };  /* Scene enviroment ambient color. */

    public: /* Application scene getter/setter functions. */
        /* Scene main camera getter function */
        const camera_component &GetMainCamera() const;

        /**
         * Set scene main camera by name function.
         *
         * \param Name - camera to set as main name.
         * \return None.
         */
        void SetMainCamera(const std::string &Name);

    private: /* Application scene methods. */
        /**
         * Perform scene rendering function.
         * 
         * \param None.
         * \return None.
         */
        void Render();

        /**
         * Perform scene script update function.
         * 
         * \param None.
         * \return None.
         */
        void CallUpdate();

    public:
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
         * Create scene object function.
         * 
         * \param Name - object name (mainly for debug purpose).
         * \return created object.
         */
        scene_object CreateObject(const std::string &Name = "");
    };
}
