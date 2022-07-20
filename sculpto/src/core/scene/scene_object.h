/*****************************************************************//**
 * \file   scene_object.h
 * \brief  Application scene object class defintion module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "scene.h"
#include "../components/native_script_component.h"

namespace scl
{
    /* Appliaction scene object class. */
    class scene_object
    {
        friend class scene;
        friend class scene_hierarchy_window;

    private: /* Scne object data. */
        scene_object_handle Entity { entt::null };
        scene *Scene {};

    private: /* Scene object methods. */
        /**
         * Scene object constructor by entity and scene.
         * 
         * \param Entity - entity for scene object.
         * \param Scene - scene connected to scene object.
         */
        scene_object(scene_object_handle Entity, scene *Scene)
            : Entity(Entity), Scene(Scene) {}

    public:
        /* Scene object default constructor. */
        scene_object() = default;

        /* Scene object default copy constructor. */
        scene_object(const scene_object &Other) = default;

        /* scene object default destructor.*/
        ~scene_object() = default;

        /**
         * Scene object inner scene_object_handle getter function.
         * 
         * \param None.
         * \return object inner scene_object_handle.
         */
        scene_object_handle GetHandle() const { return Entity; }

        /**
         * Check if scene object is valid function.
         * 
         * \param None.
         * \return wheather object is valid or not.
         */
        bool IsOk() const { return Scene && Scene->Registry.valid(Entity); }

        /**
         * Check if object has specified component function.
         *
         * \tparam component to check.
         * \param None.
         * \return None.
         */
        template <typename T>
        const bool HasComponent() const
        {
            return Scene->Registry.all_of<T>(Entity);
        }

        /**
         * Add component to object function.
         *
         * \tparam component to add to object.
         * \param None.
         * \return None.
         */
        template <typename T, typename... Targs>
        decltype(auto) AddComponent(Targs&&... Args)
        {
            return Scene->Registry.emplace<T>(Entity, std::forward<Targs>(Args)...);
        }

        /**
         * Get object component function.
         *
         * \tparam component to get from object.
         * \param None.
         * \return None.
         */
        template <typename T>
        T &GetComponent()
        {
            SCL_CORE_ASSERT(HasComponent<T>(), "Scene object don't have \"{}\" component!", typeid(T).name());
            return Scene->Registry.get<T>(Entity);
        }

        /**
         * Gect object component function.
         *
         * \tparam component to get from object.
         * \param None.
         * \return None.
         */
        template <typename T>
        const T &GetComponent() const 
        {
            SCL_CORE_ASSERT(HasComponent<T>(), "Scene object don't have \"{}\" component!", typeid(T).name());
            return Scene->Registry.get<T>(Entity);
        }

        /**
         * Remove component from scene object function.
         * 
         * \tparam component to remove from object.
         * \param None.
         * \return None.
         */
        template <typename T>
        void RemovetComponent()
        {
            Scene->Registry.remove<T>(Entity);
        }

        /**
         * Check if scene object exists function.
         * 
         * \param None.
         * \return wheather scene object exists or not.
         */
        operator bool() const { return Entity != entt::null; }

        /**
         * Getting scene object inner scene_object_handle.
         * 
         * \param None.
         * \return inner object scene_object_handle.
         */
        operator u32() const { return (u32)Entity; }

        /**
         * Getting scene object inner scene_object_handle.
         *
         * \param None.
         * \return inner object scene_object_handle.
         */
        operator scene_object_handle() const { return Entity; }

        /**
         * Scene objects compare function.
         * 
         * \param Other - scene object to compare with.
         * \return wheather scene objects are the same or not.
         */
        bool operator==(const scene_object &Other) const
        {
            return Entity == Other.Entity && Scene == Other.Scene;
        }

        /**
         * Scene objects compare function.
         *
         * \param Other - scene object to compare with.
         * \return wheather scene objects are the same or not.
         */
        bool operator!=(const scene_object &Other) const
        {
            return Entity != Other.Entity || Scene != Other.Scene;
        }
    };
}
