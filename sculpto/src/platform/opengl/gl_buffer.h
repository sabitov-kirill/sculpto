/*!****************************************************************//*!*
 * \file   buffer.h
 * \brief  Opengl buffer class definition module.
 *
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include "gl.h"
#include "core/render/primitives/buffer.h"

namespace scl
{
    /*! Constant (uniform) buffer class. */
    class gl_constant_buffer : public constant_buffer
    {
    private: /*!  Constant (uniform) buffer data. */
        mutable u32 BindingPoint {};
        GLuint Id {};
        u32 Size {};

    public:
        /*! Backend api render primitive hadnle getter function. */
        render_primitive::handle GetHandle() const override { return Id; }

        /*!*
         * Constant (uniform) empty buffer constructor.
         *
         * \param BindingPoint - shader_props binding point of buffer.
         * \param Size - buffer data size.
         */
        gl_constant_buffer(u32 Size);

        /*!*
         *  Constant (uniform) buffer filled with data constructor.
         *
         * \param BindingPoint - shader_props binding point of buffer.
         * \param Data - buffer data pointer.
         * \param Size - buffer data size.
         */
        gl_constant_buffer(const void *Data, u32 Size);

        /*! Constant buffer default destructor. */
        ~gl_constant_buffer() override;

        /*!*
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind(u32 BindingPoint) const override;

        /*!*
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;

        /*!*
         * Update buffer data function.
         *
         * \param Data - buffer data pointer.
         * \param Size - buffer data size.
         * \return None.
         */
        void Update(void *Data, u32 Size) override;

        /*!*
         * Clear buffer from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        void Free() override;
    };

    /*! Vertex bufer interface. */
    class gl_vertex_buffer : public vertex_buffer
    {
    private: /*! Vertex buffer data. */
        GLuint Id {};
        u32 VerticesCount {};

    public: /*! Vertex buffer data getter/setter functions. */
        /*! Vertex buffer vertices count getter function. */
        u32 GetCount() const override { return VerticesCount; }

    public:
        /*! Backend api render primitive hadnle getter function. */
        render_primitive::handle GetHandle() const override { return Id; }

        /*!*
         * Empty vertex buffer constructor.
         *
         * \param Count - vertices in buffer count.
         * \param VertexLayout - layout of vertices in buffer.
         * \return constant buffer pointer.
         */
        gl_vertex_buffer(u32 Count, const vertex_layout &VertexLayout);

        /*!*
         *  Vertex buffer filled with data constructor.
         *
         * \param Vertices - buffer data pointer.
         * \param Count - vertices in buffer count.
         * \param VertexLayout - layout of vertices in buffer.
         */
        gl_vertex_buffer(const void *Vertices, u32 Count, const vertex_layout &VertexLayout);

        /*! Vertex buffer default destructor. */
        ~gl_vertex_buffer();

        /*!*
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override;

        /*!*
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;

        /*!*
         * Update buffer data function.
         *
         * \param Vertices - verices array.
         * \param Count - length of vertices array.
         * \return None.
         */
        void Update(const void *Vertices, u32 Count) override;

        /*!*
         * Clear buffer from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        void Free() override;
    };

    /*! Vertices indices buffer interface. */
    class gl_index_buffer : public index_buffer
    {
    private: /*! Index buffer data. */
        GLuint Id {};
        u32 IndicesCount {};

    public: /*! Index buffer getter/setter functions. */
        /*! Index buffer indices count getter function. */
        u32 GetCount() const override { return IndicesCount; };

        /*! Backend api render primitive hadnle getter function. */
        render_primitive::handle GetHandle() const override { return Id; }

    public:
        /*!*
         * Create API specific vertex buffer by vertices count and data.
         *
         * \param Indices - indices array.
         * \param Count - indices in buffer count.
         */
        gl_index_buffer(u32 *Indices, u32 Count);

        /*! Index buffer default destructor. */
        ~gl_index_buffer() override;

        /*!*
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override;

        /*!*
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;

        /*!*
         * Update buffer function function.
         * 
         * \param Indices - array of indices.
         * \param Count
         */
        void Update(u32 *Indices, u32 Count) override;

        /*!*
         * Clear buffer from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        void Free() override;
    };
}
