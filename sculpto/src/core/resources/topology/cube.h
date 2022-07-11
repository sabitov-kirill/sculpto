/*****************************************************************//**
 * \file   cube.h
 * \brief  Topology cube object class declaration function.
 * 
 * \author Sabitov Kirill
 * \date   29 June 2022
 *********************************************************************/

#pragma once

#include "trimesh.h"
#include "../vertex.h"

namespace scl::topology
{
    /* Topology cube object class. */
    class cube : public trimesh
    {
    public:
        /**
         * Cube topology object constructor by two points.
         * 
         * \param A
         * \param B
         */
        cube(const vec3 &A, const vec3 &B);

        /* Cube topology object default destructor. */
        ~cube() override = default;
    };
}
