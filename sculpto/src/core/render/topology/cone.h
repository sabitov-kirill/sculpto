/*****************************************************************//**
 * \file   cone.h
 * \brief  Topology cone object class definition module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "grid.h"

namespace scl::topology
{
    /* Topology cone object class. */
    class cone : public grid
    {
    public:
        /**
         * Topology cone object class contructor.
         * 
         * \param A - cone bottom base middle point.
         * \param R1 - cone bottmo base cone
         * \param B - cone top base middle point.
         * \param R2 - cone top base middle point.
         * \param Slices - mesh slices count.
         */
        cone(const vec3 &A, float R1, const vec3 &B, float R2, int Slices);
    };
}
