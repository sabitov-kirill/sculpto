/*****************************************************************//**
 * \file   cube.h
 * \brief  Topology cube object class declaration function.
 * 
 * \author Sabitov Kirill
 * \date   29 June 2022
 *********************************************************************/

#pragma once

#include "basis.h"
#include "../vertex.h"

namespace scl::topology
{
    /* Topology cube object class. */
    class cube : public basis<vertex>
    {
    public: /* Cube data getter/setter functions. */
        /* Topology vertices color setter function. */
        void SetColor(const vec4 &Color)
        {
            for (size_t i = 0; i < Vertices.size(); i++)
                Vertices[i].Color = Color;
        }

    public:
        /**
         * Cube topology object constructor by two points.
         * 
         * \param A
         * \param B
         */
        cube(const vec3 &A, const vec3 &B);
    };
}
