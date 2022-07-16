/*****************************************************************//**
 * \file   camera_component.h
 * \brief  Application scene system object camera component class implementation module.
 *
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "core/resources/camera.h"

namespace scl
{
    /* Application scene system object camera component class. */
    struct camera_component
    {
        /* Camera component actual camera object. */
        camera Camera { camera_projection_type::PERSPECTIVE };
        /* Flag, showing wheather camera is primary on scene.
         * If true scene is rendered to frame buffers of this scene.
         * If multiple cameras set as primary firest (in order of adding to scene) is used for render.
         */
        bool IsPrimary { false };

        camera_component() = default;
        camera_component(const camera_component &Other) = default;
        camera_component(const camera &Camera, bool IsPrimary = false) :
            Camera(Camera), IsPrimary(IsPrimary) {}
        ~camera_component() = default;
    };
}