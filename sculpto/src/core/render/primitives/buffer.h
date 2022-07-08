/*****************************************************************//**
 * \file   buffer.h
 * \brief  Buffer interfaces definition module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include "render_primitive.h"
#include "../render_context.h"
#include "../vertex.h"

namespace scl
{
    /* Buffer usage enum. */
    enum class usage_type: u8
    {
        STREAM_DRAW,
        STREAM_READ,
        STREAM_COPY,
        STATIC_DRAW,
        STATIC_READ,
        STATIC_COPY,
        DYNAMIC_DRAW,
        DYNAMIC_READ,
        DYNAMIC_COPY,
    };

    /* Buffer type enum. */
    enum class buffer_type: u8
    {
        ARRAY,
        ELEMENT_ARRAY,
        SHADER_STORAGE,
    };

    /* Constant buffer (uniform buffer) interface. */
    class constant_buffer : public render_primitive
    {
    public:
        /* Constant buffer default destructor. */
        virtual ~constant_buffer() = default;

        /**
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind(u32 BindingPoint) const = 0;

        /**
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /**
         * Update buffer data function.
         *
         * \param Data - buffer data pointer.
         * \param Size - buffer data size.
         * \return None.
         */
        virtual void Update(void *Data, u32 Size) = 0;

        /**
         * Clear buffer from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /**
         * Create API specific empty constant buffer.
         *
         * \param BindingPoint - shader_props binding point of buffer.
         * \param Size - buffer data size.
         * \return constant buffer pointer.
         */
        static shared<constant_buffer> Create(u32 Size);

        /**
         * Create API specific constant buffer filled with data.
         *
         * \param BindingPoint - shader_props binding point of buffer.
         * \param Data - buffer data pointer.
         * \param Size - buffer data size.
         * \return constant buffer pointer.
         */
        static shared<constant_buffer> Create(const void *Data, u32 Size);
    };

    /* Vertex bufer interface. */
    class vertex_buffer : public render_primitive
    {
    protected: /* Vertex buffer data. */
        vertex_layout VertexLayout {};

    public: /* Vertex buffer getter/setter functions. */
        /* Vertex layout data getter function. */
        vertex_layout GetVertexLayout() const { return VertexLayout; }

    public:
        /**
         * Vertex buffer default constructor.
         * 
         * \param VertexLayout - layout of vertices in buffer.
         */
        vertex_buffer(const vertex_layout &VertexLayout);

        /* Vertex buffer default destructor. */
        virtual ~vertex_buffer() = default;

        /**
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const = 0;

        /**
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /**
         * Update buffer data function.
         * 
         * \param Vertices - verices array.
         * \param Count - length of vertices array.
         * \return None.
         */
        virtual void Update(const void *Vertices, u32 Count) = 0;

        /**
         * Clear buffer from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /**
         * Vertex biffer vertices count getter function.
         *
         * \param None.
         * \return vertices count.
         */
        virtual u32 GetCount() const = 0;

        /**
         * Create API specific vertex buffer by vertices count.
         * 
         * \param Count - vertices in buffer count.
         * \param VertexLayout - layout of vertices in buffer.
         * \return vertex buffer pointer.
         */
        static shared<vertex_buffer> Create(u32 Count, const vertex_layout &VertexLayout);

        /**
         * Create API specific vertex buffer by vertices count and data.
         * 
         * \param Vertices - vertices array.
         * \param Count - vertices in buffer count.
         * \param VertexLayout - layout of vertices in buffer.
         * \return vertex buffer pointer.
         */
        static shared<vertex_buffer> Create(const void *Vertices, u32 Count, const vertex_layout &VertexLayout);
    };

    /* Vertices indices buffer interface. */
    class index_buffer : public render_primitive
    {
    public:
        /* Index buffer default destructor. */
        virtual ~index_buffer() = default;

        /**
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const = 0;

        /**
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /**
         * Clear buffer from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /**
         * Index biffer indices count getter function.
         * 
         * \param None.
         * \return indicies count.
         */
        virtual u32 GetCount() const = 0;

        /**
         * Create API specific index buffer by indices count and data.
         *
         * \param Indices - indices array.
         * \param Count - indices in buffer count.
         * \return vertex buffer pointer.
         */
        static shared<index_buffer> Create(u32 *Indices, u32 Count);
    };
}
