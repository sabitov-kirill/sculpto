/*!****************************************************************//*!*
 * \file   grid.cpp
 * \brief  Topology grid object class definition module.
 * 
 * \author Sabitov Kirill
 * \date   29 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "grid.h"

scl::topology::grid::grid(int Width, int Height) :
    basis(mesh_type::TRIANGLES), Width(Width), Height(Height)
{
    for (int i = 0; i < Height; ++i)
        for (int j = 0; j < Width; ++j)
        {
            Indices.push_back((i + 1) * (Width + 1) + j + 1);
            Indices.push_back((i + 0) * (Width + 1) + j + 1);
            Indices.push_back((i + 0) * (Width + 1) + j + 0);
            Indices.push_back((i + 0) * (Width + 1) + j + 0);
            Indices.push_back((i + 1) * (Width + 1) + j + 0);
            Indices.push_back((i + 1) * (Width + 1) + j + 1);
        }

    Min = vec3(0), Max = vec3((float)Width, 0, (float)Height);
}
