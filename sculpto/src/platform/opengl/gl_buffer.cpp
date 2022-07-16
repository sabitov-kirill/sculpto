/*****************************************************************//**
 * \file   gl_buffer.cpp
 * \brief  Opengl buffer class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "gl_buffer.h"

scl::gl_constant_buffer::gl_constant_buffer(u32 Size)
{
    this->Size = Size;

    glGenBuffers(1, &Id);
    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBufferData(GL_UNIFORM_BUFFER, Size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    SCL_CORE_SUCCES("OpenGL Constant buffer with id {} and size {} created.", Id, Size);
}

scl::gl_constant_buffer::gl_constant_buffer(const void *Data, u32 Size)
{
    this->Size = Size;

    glGenBuffers(1, &Id);
    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBufferData(GL_UNIFORM_BUFFER, Size, Data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    SCL_CORE_SUCCES("OpenGL Constant buffer with id {} and size {} created.", Id, Size);
}

scl::gl_constant_buffer::~gl_constant_buffer()
{
    Free();
}

void scl::gl_constant_buffer::Bind(u32 BindingPoint) const
{
    this->BindingPoint = BindingPoint;
    if (Id != 0) glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, Id);
}

void scl::gl_constant_buffer::Unbind() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, BindingPoint, 0);
    this->BindingPoint = 0;
}

void scl::gl_constant_buffer::Update(void *Data, u32 Size)
{
    if (Id != 0)
    {
        SCL_CORE_ASSERT(this->Size == Size, "Constant buffer size can't be changed.");

        glBindBuffer(GL_UNIFORM_BUFFER, Id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, Size, Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void scl::gl_constant_buffer::Free()
{
    if (Id != 0)
    {
        glDeleteBuffers(1, &Id);

        SCL_CORE_INFO("OpenGL Constant buffer with id {} freed.", Id);
        Id = 0, BindingPoint = 0, Size = 0;
    }
}

scl::gl_vertex_buffer::gl_vertex_buffer(u32 Count, const vertex_layout &VertexLayout) :
    vertex_buffer(VertexLayout)
{
    this->VerticesCount = Count;

    glGenBuffers(1, &Id);
    glBindBuffer(GL_ARRAY_BUFFER, Id);
    glBufferData(GL_ARRAY_BUFFER, Count, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    SCL_CORE_SUCCES("OpenGL Vertex buffer with id {} and {} verices created.", Id, Count);
}

scl::gl_vertex_buffer::gl_vertex_buffer(const void *Vertices, u32 Count, const vertex_layout &VertexLayout) :
    vertex_buffer(VertexLayout)
{
    this->VerticesCount = Count;

    glCreateBuffers(1, &Id);
    glBindBuffer(GL_ARRAY_BUFFER, Id);
    glBufferData(GL_ARRAY_BUFFER, (u64)Count * VertexLayout.GetSize(), Vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    SCL_CORE_SUCCES("OpenGL Vertex buffer with id {} and {} verices created.", Id, Count);
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
        SCL_CORE_ASSERT(Count == this->VerticesCount, "Vertices count can't be changed.");

        glBindBuffer(GL_ARRAY_BUFFER, Id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (u64)Count * VertexLayout.GetSize(), Vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void scl::gl_vertex_buffer::Free()
{
    if (Id != 0)
    {
        glDeleteBuffers(1, &Id);

        SCL_CORE_INFO("OpenGL Vertex buffer with id {} freed.", Id);
        Id = 0, VerticesCount = 0;
    }
}

scl::gl_index_buffer::gl_index_buffer(u32 *Indices, u32 Count)
{
    this->IndicesCount = Count;

    glCreateBuffers(1, &Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(u32), Indices, GL_STATIC_DRAW);

    SCL_CORE_SUCCES("OpenGL Index buffer with id {} and {} indices created.", Id, Count);
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

void scl::gl_index_buffer::Update(u32 *Indices, u32 Count)
{
    if (Id != 0)
    {
        SCL_CORE_ASSERT(this->IndicesCount == Count, "Indices count cant'be changed.");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(u32), Indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void scl::gl_index_buffer::Free()
{
    if (Id != 0)
    {
        glDeleteBuffers(1, &Id);

        SCL_CORE_INFO("OpenGL Index buffer with id {} freed.", Id);
        Id = 0, IndicesCount = 0;
    }
}
