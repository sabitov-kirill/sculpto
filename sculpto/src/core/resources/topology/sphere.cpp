/*!****************************************************************//*!*
 * \file   sphere.cpp
 * \brief  Topology sphere object class implementation class.
 * 
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "sphere.h"

scl::topology::sphere::sphere(const vec3 &Center, float Radius, int Slices) :
    grid(Slices, Slices)
{
    for (int i = 0; i <= Slices; i++)
    {
        float theta = (float)i / Slices * math::PI;
        for (int j = 0; j <= Slices; j++)
        {
            float phi = (float)j / Slices * 2 * math::PI;
            float x = sin(phi) * sin(theta);
            float y = cos(phi);
            float z = sin(phi) * cos(theta);
            float x_tbn = sin(phi) * cos(theta);
            float y_tbn = cos(phi) * cos(theta);
            vec3 position { x, y, z };
            vec3 tangent   = position.Cross({ x_tbn, 0, y_tbn }).Normalized();
            vec3 bitangent = tangent.Cross(position).Normalized();
            vec2 texture_coordinates { (float)i / Slices, (float)j / Slices };

            Vertices.emplace_back(
                position * Radius + Center, position,
                tangent, bitangent,
                texture_coordinates
            );
        }
    }

    Min = -vec3(Radius), Max = vec3(Radius);
}
