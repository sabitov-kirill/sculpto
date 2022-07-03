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

    private:
        entt::entity Entity { entt::null };
        scene *Scene {};

        scene_object(entt::entity Entity, scene *Scene)
            : Entity(Entity), Scene(Scene) {}

    public:
        scene_object() = default;
        scene_object(const scene_object &Other) = default;
        ~scene_object() = default;

        template <typename T>
        const bool HasComponent() const
        {
            return Scene->Registry.all_of<T>(Entity);
        }

        template <typename T, typename... Targs>
        T &AddComponent(Targs&&... Args)
        {
            SCL_CORE_ASSERT(!HasComponent<T>(), "Entity already have this component!");
            return Scene->Registry.emplace<T>(Entity, std::forward<Targs>(Args)...);
        }

        template <typename T>
        T &GetComponent()
        {
            SCL_CORE_ASSERT(HasComponent<T>(), "Entity do not have this component!");
            return Scene->Registry.get<T>(Entity);
        }

        template <typename T>
        const T &GetComponent() const 
        {
            SCL_CORE_ASSERT(HasComponent<T>(), "Entity do not have this component!");
            return Scene->Registry.get<T>(Entity);
        }

        template <typename T>
        void RemovetComponent()
        {
            SCL_CORE_ASSERT(HasComponent<T>(), "Entity do not have this component!");
            Scene->Registry.remove<T>(Entity);
        }

        operator bool() const { return Entity != entt::null; }
    };
}
