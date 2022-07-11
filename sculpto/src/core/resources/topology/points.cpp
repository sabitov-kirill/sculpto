/*****************************************************************//**
 * \file   points.cpp
 * \brief  Topology objects points array (useful for creation dummy rendering object) class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   10 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "points.h"

scl::topology::points::points() : basis(mesh_type::POINTS)
{
    Vertices.push_back({ vec3 { 0 }, vec3 { 0 } });
    Indices.push_back(0);
}

scl::topology::points::points(const std::vector<vertex_point> &Points) : basis(mesh_type::POINTS)
{
    Vertices = Points;
    Indices.resize(Points.size());
    for (int i = 0; i < Points.size(); i++)
        Indices.push_back(i);
}
