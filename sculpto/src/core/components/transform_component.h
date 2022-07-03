/*****************************************************************//**
 * \file   transform_component.h
 * \brief  Application scene system objects transform component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Application scene system objects transform component class. */
    struct transform_component
    {
        matr4 Transform {};

        transform_component() = default;
        transform_component(const transform_component &Other) : Transform(Other.Transform) {}
        transform_component(const matr4 &Tranform) : Transform(Transform) {}
        transform_component(const vec3 &Scale, const vec3 &Angles, const vec3 &Position)
        {
            Transform =
                matr4::Scale(Scale) *
                matr4::RotateX(Angles.X) *
                matr4::RotateY(Angles.Y) *
                matr4::RotateZ(Angles.Z) *
                matr4::Translate(Position);
        }
        ~transform_component() = default;

        operator const matr4 &() const { return Transform; }
        operator matr4 &() { return Transform; }
    };
}
