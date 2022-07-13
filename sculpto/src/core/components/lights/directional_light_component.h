/*****************************************************************//**
 * \file   directional_light_component.h
 * \brief  Directional light component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "core/render/primitives/frame_buffer.h"

namespace scl
{
    /* Directional light component class. */
    struct directional_light_component
    {
        /* Light direction. */
        vec3 Direction {};

        /* Light color. */
        vec3 Color {};

        /* Shadow map data. */
        shared<frame_buffer> ShadowMap {};
        matr4 Projection {};
        bool IsShadows {};
        float BoxSize {};
        float Distance {};

        directional_light_component() = default;
        directional_light_component(const directional_light_component &Other) = default;
        directional_light_component(const vec3 &Direction, const vec3 &Color, bool IsShadows = false, float BoxSize = 10, float Distance = 0,
                                    int ShadowMapWidth = 1000, int ShadowMapHeight = 1000) :
            Direction(Direction.Normalized()), Color(Color), IsShadows(IsShadows), BoxSize(BoxSize), Distance(Distance)
        {
            if (IsShadows)
            {
                Projection = matr4::Ortho(-BoxSize, BoxSize, -BoxSize, BoxSize, 1, Distance);
                ShadowMap = frame_buffer::Create(frame_buffer_props { ShadowMapWidth, ShadowMapHeight, 1, false, 0, 1 });
            }
        }
        ~directional_light_component() = default;
    };
}
