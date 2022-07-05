/*****************************************************************//**
 * \file   light.h
 * \brief  Rendererer light base object class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   04 July 2022
 *********************************************************************/

#pragma once

#include "../primitives/shader.h"

namespace scl
{
    /* Rendererer light base object class. */
    class light
    {
    private: /* Light object default data. */
        vec3 Position;
        vec3 Color;

    public:
        /**
         * Light object default constructor.
         * 
         * \param Position - light object position.
         * \param Color - light object lighting color.
         */
        light(const vec3 &Position, const vec3 &Color)
            : Position(Position), Color(Color) {}
    };
}
