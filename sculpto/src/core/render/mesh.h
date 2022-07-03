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

#include "topology/basis.h"
#include "primitives/vertex_array.h"
#include "primitives/buffer.h"

namespace scl
{
    /* Mesh class. */
    class mesh
    {
    private: /* Mesh data. */
        shared<vertex_array> VertexArray;

    public: /* Mesh getter/setter functions. */
        /* Vertex array getter function. */
        const shared<vertex_array> &GetVertexArray() const { return VertexArray; }

    public:
        /**
         * Mesh constructor by topology object and material.
         *
         * \param TopologyObject - topology object to create vertex array from.
         */
        template <typename Tvertex>
        mesh(const topology::basis<Tvertex> &TopologyObject)
        {
            VertexArray = vertex_array::Create(
                TopologyObject.GetType(),
                vertex_buffer::Create(TopologyObject.GetVertices().data(), TopologyObject.GetVertices().size(), vertex::GetVertexLayout()),
                index_buffer::Create((u32 *)TopologyObject.GetIndices().data(), TopologyObject.GetIndices().size()) 
            );
        }

    public:
        /**
         * Mesh constructor by topology object and material.
         *
         * \param TopologyObject - topology object to create vertex array from.
         * \return created mesh pointer.
         */
        template <typename Tvertex>
        static shared<mesh> Create(const topology::basis<Tvertex> &TopologyObject)
        {
            return CreateShared<mesh>(TopologyObject);
        }
    };
}
