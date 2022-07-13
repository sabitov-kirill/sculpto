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
        camera Camera { camera_projection_type::PERSPECTIVE };

        camera_component() = default;
        camera_component(const camera &Camera) : Camera(Camera) {}
        camera_component(const camera_component &Other) = default;
        ~camera_component() = default;
    };
}