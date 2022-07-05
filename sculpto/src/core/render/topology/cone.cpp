/*****************************************************************//**
 * \file   cone.cpp
 * \brief  Topology cone object class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "cone.h"

scl::topology::cone::cone(const vec3 &A, float R1, const vec3 &B, float R2, int Slices) :
    grid(Slices, Slices)
{
    float total_height = (A - B).Length();
    vec3 height_dir = (A - B).Normalized();
    vec3 height_dir_90 = vec3 { -height_dir.Y, height_dir.X, height_dir.Z };
    vec3 guide = height_dir_90 - height_dir;
    float cos_alpha = height_dir_90.Dot(guide);
    float sin_alpha = sqrt(1 - cos_alpha * cos_alpha);
    vec3 normal = vec3(cos_alpha, sin_alpha, 0).Normalized();

    for (int i = 0; i <= Slices; i++)
        for (int j = 0; j <= Slices; j++)
        {
            float t1 = (float)j / Slices, t2 = (float)i / Slices;
            float phi = t1 * 360;
            float current_radius = math::Lerp(R1, R2, 1 - t2);
            float current_height = total_height * t2;
            matr4 rotation = matr4::Rotate(height_dir, phi);

            vertex vert;
            vert.Position = A + height_dir * current_height + rotation.TransformVector(height_dir_90 * current_radius);
            vert.Normal = rotation.TransformVector(normal);
            vert.TexCoords = { t2, t1 };
            Vertices[i * (Slices + 1) + j] = vert;
        }
}
