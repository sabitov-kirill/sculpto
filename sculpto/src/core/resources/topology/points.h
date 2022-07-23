/*!****************************************************************//*!*
 * \file   points.h
 * \brief  Topology objects points array (useful for creation dummy rendering object) class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   10 July 2022
 *********************************************************************/

#pragma once

#include "basis.h"
#include "../vertex.h"

namespace scl::topology
{
    /*! Topology objects points array class. */
    class points : public basis<vertex_point>
    {
    public:
        /*!*
         * Topology point array object default contructor (creates one point).
         * 
         * \param None.
         */
        points();

        /*!*
         * Topology point array object contructor by array of points.
         * 
         * \param Points - array of points.
         */
        points(const std::vector<vertex_point> &Points);
    };
}
