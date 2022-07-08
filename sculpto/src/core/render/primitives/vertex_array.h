/*****************************************************************//**
 * \file   mesh.h
 * \brief  Mesh interfaces definition module.
 *         Mesh stores vertex and index buffer and implement their binding during render.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#pragma once

#include "buffer.h"

namespace scl
{
    /* Mesh types enum. */
    enum class mesh_type
    {
        LINES,        /* Line segments (by 2 points) */
        POINTS,       /* Array of points */
        PATCHES,      /* Tesselation patchesz */
        TRIANGLES,    /* Triangle mesh - array of triangles */
        TRISTRIP,     /* Triangle strip - array of stripped triangles */
    };

    /* Mesh interface. */
    class vertex_array : public render_primitive
    {
    private: /* Vertex array data. */
        mesh_type Type;
        shared<vertex_buffer> VertexBuffer {};
        shared<index_buffer> IndexBuffer {};

    public: /* Vertex array getter/setter functions. */
        /* Mesh type getter function. */
        mesh_type GetType() const { return Type; }
        /* Vertex buffer, linked to vertex array getter function. */
        const shared<vertex_buffer> &GetVertexBuffer() const { return VertexBuffer; }
        /* Index buffer, linked to vertex array getter function. */
        const shared<index_buffer> &GetIndexBuffer() const { return IndexBuffer; }

    public:
        /**
         * Vertex buffer default constructor.
         * 
         * \param Type - creating mesh type.
         * \param VertexBuffer - vertex buffer to be linked to vertex array.
         * \param IndexBuffer - index array to be linked to vertex array.
         */
        vertex_array(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer);

        /**
         * Bind vertex array to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const = 0;

        /**
         * Unbind vertex array from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /**
         * Create vertex array function.
         * 
         * \param Type - creating mesh type.
         * \param VertexBuffer - vertex buffer to link to vertex array.
         * \param IndexBuffer - vartex indicies buffer to be linked to vertex array.
         * \return pointer to created vertex array.
         */
        static shared<vertex_array> Create(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer);
    };
}
