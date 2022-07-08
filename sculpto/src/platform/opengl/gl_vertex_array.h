/*****************************************************************//**
 * \file   mesh.h
 * \brief  OpenGL mesh (vertex_array) class definition module.
 *         Mesh stores vertex and index buffer and implement their binding during render.
 *
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#pragma once

#include "gl.h"
#include "core/render/primitives/vertex_array.h"

namespace scl
{
    /* OpenGL vertex array class. */
    class gl_vertex_array: public vertex_array
    {
    private: /* Vertex array data. */
        GLuint Id {};

        /**
         * Convert abstract shader variable type to OpenGL specific.
         *
         * \param Type - shader variable type to convert to OpenGL specific.
         * \return OpenGL specific shader variable type.
         */
        constexpr static GLenum GetGLShaderVariableType(shader_variable_type Type);

    public:
        /* Backend api render primitive hadnle getter function. */
        render_primitive::handle GetHandle() const override { return Id; }

        /**
         * Vertex buffer default constructor.
         *
         * \param Type - creating mesh type.
         * \param VertexBuffer - vertex buffer to be linked to vertex array.
         * \param IndexBuffer - index array to be linked to vertex array.
         */
        gl_vertex_array(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer);

        /* Default destructor. */
        ~gl_vertex_array();

        /**
         * Bind vertex array to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override;

        /**
         * Unbind vertex array from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;
    };
}
