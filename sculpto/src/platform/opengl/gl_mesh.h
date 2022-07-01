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
#include "core/render/primitives/mesh.h"

namespace scl
{
    /* OpenGL vertex array class. */
    class gl_mesh: public mesh
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
        /**
         * Vertex buffer default constructor.
         *
         * \param Type - creating mesh type.
         * \param VertexBuffer - vertex buffer to be linked to vertex array.
         * \param IndexBuffer - index array to be linked to vertex array.
         */
        gl_mesh(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer);

        /* Default destructor. */
        ~gl_mesh();

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
