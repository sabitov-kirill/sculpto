/*****************************************************************//**
 * \file   mesh.h
 * \brief  Vertex array interfaces implementation module.
 *
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "../render_context.h"
#include "mesh.h"
#include "platform/opengl/gl_mesh.h"

scl::mesh::mesh(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer) :
    Type(Type), VertexBuffer(VertexBuffer), IndexBuffer(IndexBuffer) {}

scl::shared<scl::mesh> scl::mesh::Create(mesh_type Type, shared<vertex_buffer> VertexBuffer, shared<index_buffer> IndexBuffer)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<gl_mesh>(Type, VertexBuffer, IndexBuffer);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
