/*****************************************************************//**
 * \file   scene.h
 * \brief  Scene class defintion module.
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

    using scene_object_handle = entt::entity;

    /* Scene class. */
    class scene
    {
        friend class scene_object;
        friend class scene_serializer;
        friend class scene_hierarchy_window;

    private: /* Scene data. */
        entt::registry Registry {}; /* Container for all components.
                                     * Contains both enitty datas and handles.
                                     */

        int     ViewportId { 30 };             /* Scene window viewport id. */
        int     ViewportWidth { 16 };          /* SCene viewport width. */
        int     ViewportHeight { 16 };         /* Scene viewport height. */
        vec3    EnviromentAmbient { 0.1f };    /* Scene enviroment ambient color. */
        float   UpdateDelay {};                /* Scene scripts update call timer. */

    public: /* Scene getter/setter functions. */
        /* Scene viewport id gette function. */
        int GetViewportId() const { return ViewportId; }
        /* Scene ambient color getter function. */
        const vec3 &GetEnviromentAmbient() const { return EnviromentAmbient; }

        /* Scene viewport setter function. */
        void SetViewportId(int ViewportId) { this->ViewportId = ViewportId; }
        /* Scene enviroment ambient color setter function. */
        void SetEnviromentAmbient(const vec3 &EnviromentAmbient) { this->EnviromentAmbient = EnviromentAmbient; }

    private:  /* Scene methods. */
        /**
         * Perform scene scripts OnUpdate functions calls function.
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
         * Perform scene rendering function.
         *
         * \param None.
         * \return None.
         */
        void Render();

        /**
         * Scene update function.
         *
         * \param None.
         * \return None.
         */
        void Update();

        /**
         * Create scene object function.
         * 
         * \param Name - object name (mainly for debug purpose).
         * \return created object.
         */
        scene_object CreateObject(const std::string &Name = "");

        /**
         * Get first scene object with specified name (ordered by creating time) or create new, if nothing found function.
         * 
         * \param Name - name of getting/creating object.
         * \return found/created scene object.
         */
        scene_object CreaetOrGetObject(const std::string &Name);

        /**
         * Get scene object by its handle function.
         * 
         * \param SceneObjectEntity - handle of getting scene object.
         * \return scene object.
         */
        scene_object GetSceneObject(scene_object_handle SceneObjectHandle);

        void RemoveObject(scene_object &Object);
    };
}
