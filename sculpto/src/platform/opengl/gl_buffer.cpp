/*****************************************************************//**
 * \file   gl_buffer.cpp
 * \brief  Opengl buffer class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "gl_buffer.h"

scl::gl_buffer::gl_buffer(u32 BindingPoint, u32 Size)
{
    this->Free();
    BindingPoint = BindingPoint;
    this->Size = Size;

    glGenBuffers(1, &Id);
    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBufferData(GL_UNIFORM_BUFFER, Size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

scl::gl_buffer::gl_buffer(u32 BindingPoint, const void *Data, u32 Size)
{
    Free();
    BindingPoint = BindingPoint;
    this->Size = Size;

    glGenBuffers(1, &Id);
    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBufferData(GL_UNIFORM_BUFFER, Size, Data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

scl::gl_buffer::~gl_buffer()
{
    Free();
}

void scl::gl_buffer::Bind() const
{
    if (Id != 0) glBindBuffer(GL_UNIFORM_BUFFER, Id);
}

void scl::gl_buffer::Unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void scl::gl_buffer::Update(void *Data, u32 Size)
{
    if (Id != 0)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, Id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, Size, Data);
    }
}

void scl::gl_buffer::Free()
{
    if (Id != 0)
    {
        glDeleteBuffers(1, &Id);
        Id = 0, BindingPoint = 0, Size = 0;
    }
}

scl::gl_vertex_buffer::gl_vertex_buffer(u32 Count, const vertex_layout &VertexLayout) :
    vertex_buffer(VertexLayout)
{
    this->VerticesCount = Count;

    glCreateBuffers(1, &Id);
    glBindBuffer(GL_ARRAY_BUFFER, Id);
    glBufferData(GL_ARRAY_BUFFER, Count, nullptr, GL_DYNAMIC_DRAW);
}

scl::gl_vertex_buffer::gl_vertex_buffer(const void *Vertices, u32 Count, const vertex_layout &VertexLayout) :
    vertex_buffer(VertexLayout)
{
    this->VerticesCount = Count;

    glCreateBuffers(1, &Id);
    glBindBuffer(GL_ARRAY_BUFFER, Id);
    glBufferData(GL_ARRAY_BUFFER, (u64)Count * VertexLayout.GetSize(), Vertices, GL_STATIC_DRAW);
}

scl::gl_vertex_buffer::~gl_vertex_buffer()
{
    Free();
}

void scl::gl_vertex_buffer::Bind() const
{
    if (Id != 0) glBindBuffer(GL_ARRAY_BUFFER, Id);
}

void scl::gl_vertex_buffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void scl::gl_vertex_buffer::Update(const void *Vertices, u32 Count)
{
    if (Id != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, Id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (u64)Count * VertexLayout.GetSize(), Vertices);
    }
}

void scl::gl_vertex_buffer::Free()
{
    if (Id != 0)
    {
        glDeleteBuffers(1, &Id);
        Id = 0, VerticesCount = 0;
    }
}

scl::gl_index_buffer::gl_index_buffer(u32 *Indices, u32 Count)
{
    this->IndicesCount = Count;

    glCreateBuffers(1, &Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(uint32_t), Indices, GL_STATIC_DRAW);
}

scl::gl_index_buffer::~gl_index_buffer()
{
    Free();
}

void scl::gl_index_buffer::Bind() const
{
    if (Id != 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
}

void scl::gl_index_buffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void scl::gl_index_buffer::Free()
{
    if (Id != 0)
    {
        glDeleteBuffers(1, &Id);
        Id = 0, IndicesCount = 0;
    }
}
