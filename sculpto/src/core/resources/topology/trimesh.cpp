/*****************************************************************//**
 * \file   trimesh.cpp
 * \brief  Topology object triangles mesh class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "trimesh.h"

scl::topology::trimesh::trimesh(const std::vector<vertex> &Vertices, const std::vector<u32> &Indieces) :
    basis(mesh_type::TRIANGLES, Vertices, Indices)
{
}

scl::topology::trimesh::trimesh(const std::vector<vertex> &&Vertices, const std::vector<u32> &&Indieces) :
    basis(mesh_type::TRIANGLES, Vertices, Indices)
{
}

void scl::topology::trimesh::EvaluateBoundBox()
{
    vec3 min = Vertices[0].Position;
    vec3 max = Vertices[0].Position;

    for (INT i = 1; i < Vertices.size(); i++)
        min = vec3::Min(min, Vertices[i].Position),
        max = vec3::Max(max, Vertices[i].Position);
    Min = min, Max = max;
}

void scl::topology::trimesh::EvaluateNormals()
{
    for (INT i = 0; i < Indices.size(); i += 3)
    {
        vec3 A = Vertices[Indices[i + 1]].Position - Vertices[Indices[i]].Position;
        vec3 B = Vertices[Indices[i + 2]].Position - Vertices[Indices[i]].Position;
        vec3 N = A.Cross(B).Normalized();

        Vertices[Indices[i + 0]].Normal = vec3(Vertices[Indices[i + 0]].Normal + N).Normalized();
        Vertices[Indices[i + 1]].Normal = vec3(Vertices[Indices[i + 1]].Normal + N).Normalized();
        Vertices[Indices[i + 2]].Normal = vec3(Vertices[Indices[i + 2]].Normal + N).Normalized();
    }
}

void scl::topology::trimesh::EvaluateTangentSpace()
{
    bool is_ok = true;
    for (INT i = 0; i < Indices.size() && is_ok; i += 3)
        is_ok = EvaluateTriangleTangentSpace(Vertices[Indices[i + 0]],
                                             Vertices[Indices[i + 1]],
                                             Vertices[Indices[i + 2]]);
    if (is_ok)
        for (auto &V : Vertices) EvaluateVertexOrthoganalTBNBasis(V);
    else
        for (auto &V : Vertices)
        {
            vec3 N = V.Normal;

            if (N.Z > N.X && N.Z > N.Y || N.Y > N.X && N.Y > N.Z)
                /* Z or Y dominant axes */
                V.Tangent = vec3(1, 0, 0);
            else
                /* X dominant axis */
                V.Tangent = vec3(0, 1, 0);
            V.Bitangent = N.Cross(V.Tangent).Normalized();
            V.Tangent = V.Bitangent.Cross(N).Normalized();
        }
}

bool scl::topology::trimesh::EvaluateTriangleTangentSpace(vertex &P0, vertex &P1, vertex &P2)
{
    float s1 = P1.TexCoords.X - P0.TexCoords.X;
    float s2 = P2.TexCoords.X - P0.TexCoords.X;
    float t1 = P1.TexCoords.Y - P0.TexCoords.Y;
    float t2 = P2.TexCoords.Y - P0.TexCoords.Y;
    float det = s1 * t2 - s2 * t1;

    if (det != 0)
    {
        vec3 E1 = P1.Position - P0.Position;
        vec3 E2 = P2.Position - P0.Position;
        vec3 T = ((E1 * t2 - E2 * t1) / det).Normalized();
        vec3 B = ((E2 * s1 - E1 * s2) / det).Normalized();

        P0.Tangent += T; P0.Bitangent += B;
        P1.Tangent += T; P1.Bitangent += B;
        P2.Tangent += T; P2.Bitangent += B;
        return true;
    }
    return false;
}

void scl::topology::trimesh::EvaluateVertexOrthoganalTBNBasis(vertex &P)
{
    P.Normal.Normalize(), P.Tangent.Normalize(), P.Bitangent.Normalize();

    P.Tangent   = (P.Tangent   - P.Normal * P.Normal.Dot(P.Tangent)).Normalized();
    P.Bitangent = (P.Bitangent - P.Normal * P.Normal.Dot(P.Bitangent) - P.Tangent * P.Tangent.Dot(P.Bitangent)).Normalized();
}
