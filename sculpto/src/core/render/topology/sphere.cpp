/*****************************************************************//**
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
            vertex vert;

            vert.Normal = {
                sin(phi) * sin(theta),
                cos(phi),
                sin(phi) * cos(theta)
            };

            vert.Position = vert.Normal * Radius + Center;

            vert.TexCoords = { (float)j / Slices, (float)i / Slices };

            Vertices[i * (Slices + 1) + j] = vert;
        }
    }
        

    Min = -vec3(Radius), Max = vec3(Radius);
}
