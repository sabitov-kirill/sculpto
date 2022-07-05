/*****************************************************************//**
 * \file   point_light_component.h
 * \brief  Point light component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include <base.h>

/*********** Point Light Contants *************
 * Distance     Constant   Linear   Quadratic
 *    0007,       1.0,      0.7   ,   1.8
 *    0013,       1.0,      0.35  ,   0.44
 *    0020,       1.0,      0.22  ,   0.20
 *    0032,       1.0,      0.14  ,   0.07
 *    0050,       1.0,      0.09  ,   0.032
 *    0065,       1.0,      0.07  ,   0.017
 *    0100,       1.0,      0.045 ,   0.0075
 *    0160,       1.0,      0.027 ,   0.0028
 *    0200,       1.0,      0.022 ,   0.0019
 *    0325,       1.0,      0.014 ,   0.0007
 *    0600,       1.0,      0.007 ,   0.0002
 *    3250,       1.0,      0.0014,   0.000007
 */

namespace scl
{
    /* Point light component class. Transform component sets position. */
    struct point_light_component
    {
        /* Light color. */
        vec3 Color {};

        /* Point light attenuation coefficients. */
        float Constant {};
        float Linear {};
        float Quadratic {};

        point_light_component() = default;
        point_light_component(const point_light_component &Other) = default;
        point_light_component(const vec3 &Color, float Constant, float Linear, float Quadratic) :
            Color(Color), Constant(Constant), Linear(Linear), Quadratic(Quadratic) {}
        ~point_light_component() = default;
    };
}
