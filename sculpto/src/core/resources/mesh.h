/*****************************************************************//**
 * \file   mesh.h
 * \brief  Mesh class defintion module.
 *         Mesh composes vertex array (vertex and index buffer),
 *         material (shader + uniform data, textures),
 *         and tranformation (world) matrix.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "materials/material.h"
#include "topology/trimesh.h"
#include "core/render/primitives/vertex_array.h"
#include "core/render/primitives/buffer.h"

namespace scl
{
    /* Submesh creation properties type. */
    template <typename Tvertex>
    struct submesh_props
    {
        topology::trimesh Topology {}; /* Submesh topology, containing its vertices and indices. */
        shared<material> Material {};  /* Submesh material. */
    };

    /* Mesh class. */
    class mesh
    {
        friend class renderer;

        /* Single mesh data structure. */
        struct submesh_data
        {
            shared<vertex_array> VertexArray {};
            shared<vertex_buffer> VertexBuffer {};
            shared<index_buffer> IndexBuffer {};
            shared<material> Material {};
            matr4 LocalTransform {};
        };

    public:
        std::vector<submesh_data> SubMeshes {};

        /* Mesh rendering flags. */
        bool IsDrawing       = true;
        bool IsCastingShadow = true;

        /**
         * Mesh constructor by topology object and material.
         *
         * \param TopologyObject - topology object to create vertex array from.
         * \param Material - mesh material.
         */
        template <typename Tvertex>
        mesh(const topology::basis<Tvertex> &TopologyObject, shared<material> Material)
        {
            submesh_data new_sub_mesh {};
            new_sub_mesh.VertexBuffer = vertex_buffer::Create(TopologyObject.GetVertices().data(),
                                                              (u32)TopologyObject.GetVertices().size(),
                                                              Tvertex::GetVertexLayout());
            new_sub_mesh.IndexBuffer = index_buffer::Create((u32 *)TopologyObject.GetIndices().data(),
                                                            (u32)TopologyObject.GetIndices().size());
            new_sub_mesh.VertexArray = vertex_array::Create(TopologyObject.GetType(),
                                                            new_sub_mesh.VertexBuffer,
                                                            new_sub_mesh.IndexBuffer);
            new_sub_mesh.Material = Material;

            SubMeshes.push_back(new_sub_mesh);
        }

        /**
         * Mesh constructor by submeshes properties array.
         *
         * \param SubmeshesProperties - array of sub meshes data (topology object + material).
         */
        template <typename Tvertex>
        mesh(const std::vector<submesh_props<Tvertex>> &SubmeshesProperties)
        {
            for (const auto &submesh_prop : SubmeshesProperties)
            {
                submesh_data new_sub_mesh {};
                new_sub_mesh.VertexBuffer = vertex_buffer::Create(submesh_prop.Topology.GetVertices().data(),
                                                                  (u32)submesh_prop.Topology.GetVertices().size(),
                                                                  vertex::GetVertexLayout());
                new_sub_mesh.IndexBuffer = index_buffer::Create((u32 *)submesh_prop.Topology.GetIndices().data(),
                                                                (u32)submesh_prop.Topology.GetIndices().size());
                new_sub_mesh.VertexArray = vertex_array::Create(submesh_prop.Topology.GetType(),
                                                                new_sub_mesh.VertexBuffer,
                                                                new_sub_mesh.IndexBuffer);
                new_sub_mesh.Material = submesh_prop.Material;

                SubMeshes.push_back(new_sub_mesh);
            }
        }

        /**
         * Mesh constructor by topology object and material.
         *
         * \param TopologyObject - topology object to create vertex array from.
         * \return created mesh pointer.
         */
        template <typename Tvertex>
        static shared<mesh> Create(const topology::basis<Tvertex> &TopologyObject, shared<material> Material)
        {
            return CreateShared<mesh>(TopologyObject, Material);
        }

        /**
         * Mesh constructor by submeshes properties array.
         *
         * \param SubmeshesProperties - array of sub meshes data (topology object + material).
         */
        template <typename Tvertex>
        static shared<mesh> Create(const std::vector<submesh_props<Tvertex>> &SubmeshesProperties)
        {
            return CreateShared<mesh>(SubmeshesProperties);
        }
    };
}
