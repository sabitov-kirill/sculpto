/*!****************************************************************//*!*
 * \file   transform_component.h
 * \brief  Scene object transform component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /*! Application scene system objects transform component class. */
    struct transform_component
    {
        vec3 Scale { 1 };
        vec3 Angles {};
        vec3 Position {};
        matr4 ScaleMatr {};
        matr4 AnglesMatr {};
        matr4 PositionMatr {};
        matr4 Transform {};

        transform_component() = default;
        transform_component(const transform_component &Other) = default;
        transform_component(const matr4 &Tranform) : Transform(Transform) {}
        transform_component(const vec3 &Scale, const vec3 &Angles, const vec3 &Position) :
            Scale(Scale), Angles(Angles), Position(Position),
            ScaleMatr(matr4::Scale(Scale)), AnglesMatr(matr4::RotateX(Angles.X) * matr4::RotateY(Angles.Y) * matr4::RotateZ(Angles.Z)),
            PositionMatr(matr4::Translate(Position)),
            Transform(ScaleMatr * AnglesMatr * PositionMatr) {}
        ~transform_component() = default;

        void SetScale(const vec3 &Scale)
        {
            this->Scale = Scale;
            InvalidateScale();
        }
        void SetAngles(const vec3 &Angles)
        {
            this->Angles = Angles;
            InvalidateRotation();
        }
        void SetPosition(const vec3 &Position)
        {
            this->Position = Position;
            InvalidatePosition();
        }

        void InvalidateScale()
        {
            ScaleMatr = matr4::Scale(Scale);
            Transform = ScaleMatr * AnglesMatr * PositionMatr;
        }
        void InvalidateRotation()
        {
            AnglesMatr = matr4::RotateX(Angles.X) * matr4::RotateY(Angles.Y) * matr4::RotateZ(Angles.Z);
            Transform = ScaleMatr * AnglesMatr * PositionMatr;
        }
        void InvalidatePosition()
        {
            PositionMatr = matr4::Translate(Position);
            Transform = ScaleMatr * AnglesMatr * PositionMatr;
        }
        void Invalidate()
        {
            ScaleMatr = matr4::Scale(Scale);
            AnglesMatr = matr4::RotateX(Angles.X) * matr4::RotateY(Angles.Y) * matr4::RotateZ(Angles.Z);
            PositionMatr = matr4::Translate(Position);
            Transform = ScaleMatr * AnglesMatr * PositionMatr;
        }

        operator const matr4 &() const { return Transform; }
        operator matr4 &() { return Transform; }
    };
}
