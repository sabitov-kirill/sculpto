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
    grid(1, Slices)
{
    float height       = (A - B).Length();
    vec3 height_dir    = (A - B).Normalized();
    vec3 height_dir_90 = vec3 { -height_dir.Y, height_dir.X, height_dir.Z };
    vec3 guide         = height_dir_90 - height_dir;
    float cos_alpha    = height_dir_90.Dot(guide);
    float sin_alpha    = sqrt(1 - cos_alpha * cos_alpha);
    vec3 normal        = vec3(cos_alpha, sin_alpha, 0).Normalized();

    for (int i = 0; i <= Slices; i++)
    {
        float t        = (float)i / Slices;
        float phi      = 360 * t;
        float x        = sin(phi);
        float z        = cos(phi);
        matr4 rotation = matr4::Rotate(height_dir, phi);
        vec3 normal = rotation.TransformVector(normal);
        vec3 tangent = vec3 { z, 0, -x };
        vec3 bitangent = tangent.Cross(normal);

        Vertices.emplace_back(
            A + rotation.TransformVector(height_dir_90 * R1),
            normal, tangent, bitangent, vec2 { t, 0 }
        );
        Vertices.emplace_back(
            B + rotation.TransformVector(height_dir_90 * R2),
            normal, tangent, bitangent, vec2 { t, 1 }
        );
    }
}
