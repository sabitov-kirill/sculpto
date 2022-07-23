/*!****************************************************************//*!*
 * \file   plane.h
 * \brief  Topology plane object class implementation module.
 *
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "plane.h"

scl::topology::plane::plane(int Width, int Height) :
    grid(Width, Height)
{
    for (float i = 0; i <= Height; ++i)
        for (float j = 0; j <= Width; ++j)
            Vertices.push_back(vertex({ i, 0, j },
                                      { 0, 1, 0 },
                                      { 1, 0, 0 },
                                      { 0, 0, 1 },
                                      { i, j }));
}
