/*!****************************************************************//*!*
 * \file   full_screen_quad.h
 * \brief  TOpology full screen quad object class declaration module.
 *
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#pragma once

#include "grid.h"

namespace scl::topology
{
    /*! Topology full screen quad object class. */
    class full_screen_quad: public grid
    {
    public:
        /*!*
         * Topology full screen quad object default constructor.
         *
         * \param Width, Height - full_screen_quad size.
         */
        full_screen_quad();

        /*! Topology full_screen_quad object default destructor. */
        ~full_screen_quad() override = default;
    };
}

