/*!****************************************************************//*!*
 * \file   grid.h
 * \brief  Topology grid object class definition module.
 * 
 * \author Sabitov Kirill
 * \date   29 June 2022
 *********************************************************************/

#pragma once

#include "basis.h"
#include "../vertex.h"

namespace scl::topology
{
    /*! Topology grid object class. */
    class grid : public basis<vertex>
    {
    private: /*! Topology grid obect data. */
        int Width, Height;

    public: /*! Topology grid object getter/setter functions. */
        /*! Grid width getter function. */
        int GetWidth() const { return Width; }
        /*! Grid height getter function. */
        int GetHeight() const { return Height; }

    public:
        /*!*
         * Topology grid object basis constructor.
         *
         * \param Vertices - verticeies array.
         * \param Indices - vertices indices.
         */
        grid(int Width, int Height);

        /*! Topology grid object default destructor. */
        ~grid() override = default;
    };
}
