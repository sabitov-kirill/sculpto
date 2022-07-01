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

#include "material.h"
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
        shared<material> Material;
        matr4 Transform { matr4::Identity() };

    public: /* Mesh getter/setter functions. */
        /* Vertex array getter function. */
        const shared<vertex_array> &GetVertexArray() const { return VertexArray; }
        /* Mesh material getter function. */
        const shared<material> &GetMaterial() const { return Material; }
        /* Mesh material setter function. */
        void SetMaterial(shared<material> Material) { this->Material = Material; }
        /* Mesh transformation (world) matrix getter function. */
        const matr4 &GetTransform() const { return Transform; }
        /* Mesh transformation (world) matrix setter function. */
        void SetTransform(const matr4 &Transform) { this->Transform = Transform; }

    public:
        /**
         * Mesh constructor by vertex array and material.
         * 
         * \param VertexArray - mesh vertex array.
         * \param Material - mesh material.
         */
        mesh(shared<vertex_array> VertexArray, shared<material> Material) :
            VertexArray(VertexArray), Material(Material) {}

        /**
         * Mesh constructor by topology object and material.
         *
         * \param TopologyObject - topology object to create vertex array from.
         * \param Material - mesh material.
         */
        template <typename Tvertex>
        mesh(const topology::basis<Tvertex> &TopologyObject, shared<material> Material) :
            Material(Material)
        {
            VertexArray = vertex_array::Create(
                TopologyObject.GetType(),
                vertex_buffer::Create(TopologyObject.GetVertices().data(), TopologyObject.GetVertices().size(), vertex::GetVertexLayout()),
                index_buffer::Create((u32 *)TopologyObject.GetIndices().data(), TopologyObject.GetIndices().size()) 
            );
        }

    public:
        /**
         * Mesh constructor by vertex array and material.
         *
         * \param VertexArray - mesh vertex array.
         * \param Material - mesh material.
         */
        static shared<mesh> Create(shared<vertex_array> VertexArray, shared<material> Material)
        {
            return CreateShared<mesh>(VertexArray, Material);
        }

        /**
         * Mesh constructor by topology object and material.
         *
         * \param TopologyObject - topology object to create vertex array from.
         * \param Material - mesh material.
         */
        template <typename Tvertex>
        static shared<mesh> Create(const topology::basis<Tvertex> &TopologyObject, shared<material> Material)
        {
            return CreateShared<mesh>(TopologyObject, Material);
        }
    };
}
