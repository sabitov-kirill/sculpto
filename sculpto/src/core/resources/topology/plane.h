/*****************************************************************//**
 * \file   plane.h
 * \brief  TOpology plane object class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#pragma once

#include "grid.h"

namespace scl::topology
{
    /* Topology plane object class. */
    class plane : public grid
    {
    public:
        /**
         * Topology plane object default constructor.
         * 
         * \param Width, Height - plane size.
         */
        plane(int Width, int Height);

        /* Topology plane object default destructor. */
        ~plane() override = default;
    };
}
