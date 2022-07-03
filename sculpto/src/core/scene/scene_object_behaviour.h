/*****************************************************************//**
 * \file   scene_object_behaviour.h
 * \brief  Application scene object behaviour class defintion module.
 * 
 * \author Sabitov Kirill
 * \date   03 July 2022
 *********************************************************************/

#pragma once

#include "scene_object.h"

namespace scl
{
    /* Appliaction scene scriptable object class. */
    class scene_object_behaviour
    {
        friend class scene;

    private:
        scene_object Object;

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate() {}

        template <typename T>
        const bool HasComponent() const { return Object.HasComponent<T>(); }

        template <typename T, typename... Targs>
        void AddComponent(Targs&&... Args) { Object.AddComponent(std::forward<Targs>(Args)...); }

        template <typename T>
        T &GetComponent() { return Object.GetComponent<T>(); }

        template <typename T>
        const T &GetComponent() const { return Object.GetComponent<T>(); }

        template <typename T>
        void RemovetComponent() { Object.RemovetComponent<T>(); }
    };
}
