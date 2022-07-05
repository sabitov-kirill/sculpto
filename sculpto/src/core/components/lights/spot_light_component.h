/*****************************************************************//**
 * \file   spot_light_component.h
 * \brief  Spot light component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Spot light component class. Transform component sets position of light cone. */
    struct spot_light_component
    {
        /* Light color. */
        vec3 Color {};

        /* Light direction. */
        vec3 Direction {};

        /* Light cutoff angles. */
        float InnerCutoffCos {};
        float OuterCutoffCos {};
        float Epsilon {};

        spot_light_component() = default;
        spot_light_component(const spot_light_component &Other) = default;
        spot_light_component(const vec3 &Color, const vec3 &Direction, degrees InnerCutoffAngle, degrees OuterCutoffAngle) :
            Color(Color), Direction(Direction), InnerCutoffCos(cos((radians)InnerCutoffAngle)), OuterCutoffCos(cos((radians)OuterCutoffAngle)),
            Epsilon(InnerCutoffCos - OuterCutoffCos) {}
        ~spot_light_component() = default;
    };
}
