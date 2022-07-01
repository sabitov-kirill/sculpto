/*****************************************************************//**
 * \file   buffer.h
 * \brief  Buffer interfaces implementation module.
 *
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "../render_context.h"
#include "buffer.h"
#include "platform/opengl/gl_buffer.h"

scl::shared<scl::constant_buffer> scl::constant_buffer::Create(u32 BindingPoint, u32 Size)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<scl::gl_constant_buffer>(BindingPoint, Size);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}

scl::shared<scl::constant_buffer> scl::constant_buffer::Create(u32 BindingPoint, const void *Data, u32 Size)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<scl::gl_constant_buffer>(BindingPoint, Data, Size);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}

scl::vertex_buffer::vertex_buffer(const vertex_layout &VertexLayout) :
    VertexLayout(VertexLayout) {}

scl::shared<scl::vertex_buffer> scl::vertex_buffer::Create(u32 Count, const vertex_layout &VertexLayout)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<scl::gl_vertex_buffer>(Count, VertexLayout);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}

scl::shared<scl::vertex_buffer> scl::vertex_buffer::Create(const void *Vertices, u32 Count, const vertex_layout &VertexLayout)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<scl::gl_vertex_buffer>(Vertices, Count, VertexLayout);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}

scl::shared<scl::index_buffer> scl::index_buffer::Create(u32 *Indices, u32 Count)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<scl::gl_index_buffer>(Indices, Count);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
