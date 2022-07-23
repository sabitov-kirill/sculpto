/*!****************************************************************//*!*
 * \file   trimesh.h
 * \brief  Topology object triangles mesh class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#pragma once

#include "basis.h"
#include "../vertex.h"

namespace scl::topology
{
    /*! Topology object triangles mesh class. */
    class trimesh : public basis<vertex>
    {
    public:
        /*! Topology object triangles mesh default cconstructor. */
        trimesh() : basis(mesh_type::TRIANGLES) {}

        /*!*
         * Topology object triangles mesh default cconstructor.
         * 
         * \param Vertices - triangle mesh vertices.
         * \param Indieces - triangle mesh vertices indices.
         */
        trimesh(const std::vector<vertex> &Vertices, const std::vector<u32> &Indieces);

        /*!*
         * Topology object triangles mesh default cconstructor.
         *
         * \param Vertices - triangle mesh vertices.
         * \param Indieces - triangle mesh vertices indices.
         */
        trimesh(const std::vector<vertex> &&Vertices, const std::vector<u32> &&Indieces);

        /*! Topology object triangles mesh default destructor. */
        ~trimesh() override = default;

        /*!*
         * Topology object mesh bound box evaluation function.
         *
         * \param None.
         * \return None.
         */
        virtual void EvaluateBoundBox();

        /*!*
         * Topology object mesh vertices normals evaluation function.
         *
         * \param None.
         * \return None.
         */
        virtual void EvaluateNormals();

        /*!*
         * Topology object mesh vertices tangent space evaluation function.
         * 
         * \param None.
         * \return None.
         */
        virtual void EvaluateTangentSpace();

        /*!*
         * Evaluate tangent space of single triangle function.
         *
         * \param P0, P1, P2 - triangle vertices.
         * \return None.
         */
        static bool EvaluateTriangleTangentSpace(vertex &P0, vertex &P1, vertex &P2);

        /*!*
         * Evaluate orthoganal tangent space basis of vertex function.
         *
         * \param P - vertex to evaluate orthoganal tangent space basis.
         * \return None.
         */
        static void EvaluateVertexOrthoganalTBNBasis(vertex &P);
    };
}
