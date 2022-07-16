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
    private: /* Spot light ccomponent data. */
        /* Light cutoff angles. */
        float InnerCutoff {};
        float OuterCutoff {};

    public:
        float InnerCutoffCos {};
        float OuterCutoffCos {};
        float Epsilon {};

        /* Light color. */
        vec3 Color {};
        float Strength { 1 };

    public: /* Spot light component data getter/setter functions. */
        /* Light iner cutoff angle getter functions. */
        float GetInnerCutoff() const { return InnerCutoff; }
        /* Light outer cutoff angle getter functions. */
        float GetOuterCutoff() const { return OuterCutoff; }

        /* Light inner cutoff angle setter functions. */
        void SetInnerCutoff(degrees Angle) { InnerCutoff = Angle; InnerCutoffCos = cos((radians)Angle); Epsilon = InnerCutoffCos - OuterCutoffCos; }
        /* Light outer cutoff angle setter functions. */
        void SetOuterCutoff(degrees Angle) { OuterCutoff = Angle; OuterCutoffCos = cos((radians)Angle); Epsilon = InnerCutoffCos - OuterCutoffCos; }

    public:
        spot_light_component() = default;
        spot_light_component(const spot_light_component &Other) = default;
        spot_light_component(const vec3 &Color, degrees InnerCutoffAngle, degrees OuterCutoffAngle) :
            Color(Color),
            InnerCutoffCos(cos((radians)InnerCutoffAngle)), InnerCutoff(InnerCutoffAngle),
            OuterCutoffCos(cos((radians)OuterCutoffAngle)), OuterCutoff(OuterCutoffAngle),
            Epsilon(InnerCutoffCos - OuterCutoffCos) {}
        ~spot_light_component() = default;
    };
}
