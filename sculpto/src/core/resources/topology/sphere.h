/*****************************************************************//**
 * \file   sphere.h
 * \brief  Topology sphere object class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#pragma once

#include "grid.h"

namespace scl::topology
{
    /* Topology sphere object class. */
    class sphere : public grid
    {
    public:
        /**
         * Sphere topolgy obejct constructor by center point and radius.
         * 
         * \param Center - sphere center point.
         * \param Radius - sphere radius.
         * \param Slices - sphere mesh slices count.
         */
        sphere(const vec3 &Center, float Radius, int Slices);

        /* Sphere topology object default destructor. */
        ~sphere() override = default;
    };
}
