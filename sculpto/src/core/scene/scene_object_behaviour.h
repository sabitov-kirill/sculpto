/*!****************************************************************//*!*
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
    /*! Appliaction scene scriptable object class. */
    class scene_object_behaviour
    {
        friend class scene;

    private: /*! Application scene scriptable object data. */
        scene_object Object;

    protected:
        /*!*
         * Application scene scriptable object creation virtual callback function.
         * Called when object added to scene.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnCreate() {}

        /*!*
         * Application scene scriptable object destroy virtual callback fucntion.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnDestroy() {}

        /*!*
         * Application scene scriptable object update virtual callback function.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnUpdate() {}

        /*!*
         * Check if object has specified component function.
         *
         * \tparam component to check.
         * \param None.
         * \return None.
         */
        template <typename T>
        const bool HasComponent() const { return Object.HasComponent<T>(); }

        /*!*
         * Add component to object function.
         *
         * \tparam component to add to object.
         * \param None.
         * \return None.
         */
        template <typename T, typename... Targs>
        void AddComponent(Targs&&... Args) { Object.AddComponent(std::forward<Targs>(Args)...); }

        /*!*
         * Get object component function.
         *
         * \tparam component to get from object.
         * \param None.
         * \return None.
         */
        template <typename T>
        T &GetComponent() { return Object.GetComponent<T>(); }

        /*!*
         * Get object component function.
         *
         * \tparam component to get from object.
         * \param None.
         * \return None.
         */
        template <typename T>
        const T &GetComponent() const { return Object.GetComponent<T>(); }

        /*!*
         * Remove component from scene object function.
         *
         * \tparam component to remove from object.
         * \param None.
         * \return None.
         */
        template <typename T>
        void RemovetComponent() { Object.RemovetComponent<T>(); }
    };
}
