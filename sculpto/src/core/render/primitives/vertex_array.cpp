/*****************************************************************//**
 * \file   vertex_array.h
 * \brief  Vertex array interfaces implementation module.
 *
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "../render_context.h"
#include "vertex_array.h"
#include "platform/opengl/gl_vertex_array.h"

scl::vertex_array::vertex_array(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer) :
    Type(Type), VertexBuffer(VertexBuffer), IndexBuffer(IndexBuffer) {}

scl::shared<scl::vertex_array> scl::vertex_array::Create(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer)
{
    switch (render_context::GetApi())
    {
    case scl::render_context_api::OpenGL:  return CreateShared<gl_vertex_array>(Type, VertexBuffer, IndexBuffer);
    case scl::render_context_api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
