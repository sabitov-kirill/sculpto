/*****************************************************************//**
 * \file   scene.h
 * \brief  Application scene sustem class defintion module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include <entt.hpp>
#include "base.h"

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

        int     ViewportId { 30 };             /* Scene window viewport id. */
        int     ViewportWidth { 16 };          /* SCene viewport width. */
        int     ViewportHeight { 16 };         /* Scene viewport height. */
        vec3    EnviromentAmbient { 0.1f };    /* Scene enviroment ambient color. */
        float   UpdateDelay {};                /* Scene scripts update call timer. */

    public: /* Application scene getter/setter functions. */
        /* Scene viewport id gette function. */
        int GetViewportId() const { return ViewportId; }
        /* Scene ambient color getter function. */
        const vec3 &GetEnviromentAmbient() const { return EnviromentAmbient; }

        /* Scene viewport setter function. */
        void SetViewportId(int ViewportId) { this->ViewportId = ViewportId; }
        /* Scene enviroment ambient color setter function. */
        void SetEnviromentAmbient(const vec3 &EnviromentAmbient) { this->EnviromentAmbient = EnviromentAmbient; }

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
