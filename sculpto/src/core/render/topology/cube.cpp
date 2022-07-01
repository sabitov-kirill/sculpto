/*****************************************************************//**
 * \file   cube.cpp
 * \brief  Cube topology object class imlementation module.
 * 
 * \author Sabitov Kirill
 * \date   29 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "cube.h"

scl::topology::cube::cube(const vec3 &A, const vec3 &B) :
    basis(mesh_type::TRIANGLES)
{
    float ax = A.GetX(), ay = A.GetY(), az = A.GetZ();
    float bx = B.GetX(), by = B.GetY(), bz = B.GetZ();

    // Front
    Vertices.push_back(vertex(  A,            { 0, 0, -1 }, { ax, ay }));
    Vertices.push_back(vertex({ ax, by, az }, { 0, 0, -1 }, { ax, by }));
    Vertices.push_back(vertex({ bx, ay, az }, { 0, 0, -1 }, { bx, ay }));
    Vertices.push_back(vertex({ bx, by, az }, { 0, 0, -1 }, { bx, by }));

    Indices.push_back(0);
    Indices.push_back(1);
    Indices.push_back(2);
    Indices.push_back(1);
    Indices.push_back(2);
    Indices.push_back(3);

    // Right
    Vertices.push_back(vertex({ bx, ay, az}, { 1, 0, 0 }, { ay, az }));
    Vertices.push_back(vertex({ bx, by, az}, { 1, 0, 0 }, { by, az }));
    Vertices.push_back(vertex({ bx, ay, bz}, { 1, 0, 0 }, { ay, bz }));
    Vertices.push_back(vertex(  B,           { 1, 0, 0 }, { by, bz }));

    Indices.push_back(4);
    Indices.push_back(5);
    Indices.push_back(6);
    Indices.push_back(5);
    Indices.push_back(6);
    Indices.push_back(7);

    // Back
    Vertices.push_back(vertex({ bx, ay, bz }, { 0, 0, 1 }, { bx, ay }));
    Vertices.push_back(vertex( B,             { 0, 0, 1 }, { bx, by }));
    Vertices.push_back(vertex({ ax, ay, bz }, { 0, 0, 1 }, { ax, ay }));
    Vertices.push_back(vertex({ ax, by, bz }, { 0, 0, 1 }, { ax, by }));

    Indices.push_back(8);
    Indices.push_back(9);
    Indices.push_back(10);
    Indices.push_back(9);
    Indices.push_back(10);
    Indices.push_back(11);

    // Left
    Vertices.push_back(vertex({ ax, ay, bz }, { -1, 0, 0 }, { ay, bz }));
    Vertices.push_back(vertex({ ax, by, bz }, { -1, 0, 0 }, { by, bz }));
    Vertices.push_back(vertex(  A,            { -1, 0, 0 }, { ay, az }));
    Vertices.push_back(vertex({ ax, by, az }, { -1, 0, 0 }, { by, az }));

    Indices.push_back(12);
    Indices.push_back(13);
    Indices.push_back(14);
    Indices.push_back(13);
    Indices.push_back(14);
    Indices.push_back(15);

    // Top
    Vertices.push_back(vertex({ ax, by, az }, { 0, 1, 0 }, { ax, az }));
    Vertices.push_back(vertex({ ax, by, bz }, { 0, 1, 0 }, { ax, bz }));
    Vertices.push_back(vertex({ bx, by, az }, { 0, 1, 0 }, { bx, az }));
    Vertices.push_back(vertex(  B,            { 0, 1, 0 }, { bx, bz }));

    Indices.push_back(16);
    Indices.push_back(17);
    Indices.push_back(18);
    Indices.push_back(17);
    Indices.push_back(18);
    Indices.push_back(19);

    // Bottom
    Vertices.push_back(vertex(  A,            { 0, -1, 0 }, { ax, az }));
    Vertices.push_back(vertex({ ax, ay, bz }, { 0, -1, 0 }, { ax, bz }));
    Vertices.push_back(vertex({ bx, ay, az }, { 0, -1, 0 }, { bx, az }));
    Vertices.push_back(vertex({ bx, ay, bz }, { 0, -1, 0 }, { bx, bz }));

    Indices.push_back(20);
    Indices.push_back(21);
    Indices.push_back(22);
    Indices.push_back(21);
    Indices.push_back(22);
    Indices.push_back(23);
}
