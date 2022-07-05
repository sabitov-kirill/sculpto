/*****************************************************************//**
 * \file   directional_light_component.h
 * \brief  Directional light component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Directional light component class. Transform component sets direction. */
    struct directional_light_component
    {
        /* Light color. */
        vec3 Color {};

        directional_light_component() = default;
        directional_light_component(const directional_light_component &Other) = default;
        directional_light_component(const vec3 &Color) : Color(Color) {}
        ~directional_light_component() = default;
    };
}
