/*!****************************************************************//*!*
 * \file   base.h
 * \brief  Topology object basis class for mesh creating implementation module.
 * 
 * \author Sabitov Kirill
 * \date   29 June 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "core/render/primitives/vertex_array.h"

namespace scl::topology
{
    /*! Topology object basis class. */
    template <typename Tvertex>
    class basis
    {
    public: /*! Topology object basis data. */
        mesh_type MeshType;                 /*! Object mesh type. */
        std::vector<Tvertex> Vertices {};   /*! Object vertices. */
        std::vector<u32> Indices {};        /*! Object vertices indices. */
        vec3 Min {}, Max {};                /*! Object bound box points. */

    public: /*! Topology obect basis getter/setter functions. */
        /*! Topology object type getter function. */
        const mesh_type GetType() const { return MeshType; }
        /*! Topology obect basis vertices getter function. */
        const std::vector<Tvertex> GetVertices() const { return Vertices; }
        /*! Topology obect basis vertices indices getter function. */
        const std::vector<u32> GetIndices() const { return Indices; }
        /*! Object bound box minimum point getter function. */
        const vec3 &GetBoundMin() const { return Min; }
        /*! Object bound box maximum point getter function. */
        const vec3 &GetBoundMax() const { return Max; }

        std::vector<Tvertex>::const_iterator cbegin() const { return Vertices.begin(); }
        std::vector<Tvertex>::const_iterator cend() const { return Vertices.end(); }

    public:
        /*!*
         * Topology object basis default constructor.
         *
         * \param MeshType- object mesh type.
         */
        basis(mesh_type MeshType) : MeshType(MeshType) {}

        /*!*
         * Topology object basis default constructor.
         * 
         * \param MeshType- object mesh type.
         * \param Vertices - verticeies array.
         * \param Indices - vertices indices.
         */
        basis(mesh_type MeshType, const std::vector<Tvertex> &Vertices, const std::vector<u32> &Indices) :
            MeshType(MeshType), Vertices(Vertices), Indices(Indices) {}

        /*!*
         * Topology object basis constructor.
         * 
         * \param MeshType- object mesh type.
         * \param Vertices - verticeies array rvalue ref.
         * \param Indices - vertices indices rvalue ref.
         */
        basis(mesh_type MeshType, std::vector<Tvertex> &&Vertices, std::vector<u32> &&Indices) :
            Vertices(std::move(Vertices)), Indices(std::move(Indices)) {}

        /*! Topology object basis default destructor. */
        virtual ~basis() = default;
    };
}
