/*!****************************************************************//*!*
 * \file   mesh.h
 * \brief  OpenGL mesh (vertex_array) class definition module.
 *         Mesh stores vertex and index buffer and implement their binding during render.
 *
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "gl_vertex_array.h"

inline constexpr GLenum scl::gl_vertex_array::GetGLShaderVariableType(shader_variable_type Type)
{
    switch (Type)
    {
    case scl::shader_variable_type::BOOL:
        return GL_BOOL;

    case scl::shader_variable_type::INT:
    case scl::shader_variable_type::INT2:
    case scl::shader_variable_type::INT3:
    case scl::shader_variable_type::INT4:
        return GL_INT;

    case scl::shader_variable_type::FLOAT:
    case scl::shader_variable_type::FLOAT2:
    case scl::shader_variable_type::FLOAT3:
    case scl::shader_variable_type::FLOAT4:
    case scl::shader_variable_type::MATR4:
        return GL_FLOAT;
    }

    SCL_CORE_ASSERT(0, "Unknown shader veriable type.");
    return GLenum();
}

void scl::gl_vertex_array::SetVertexBuffer(const shared<vertex_buffer> &VertexBuffer)
{
    SCL_CORE_ASSERT(VertexBuffer->GetVertexLayout().GetCount() != 0,
                    "Vertex Buffer has no layout!");

    this->VertexBuffer = VertexBuffer;
    this->Bind();
    VertexBuffer->Bind();

    const auto &layout = VertexBuffer->GetVertexLayout();
    for (const auto &element : layout)
    {
        switch (element.Type)
        {
        case shader_variable_type::BOOL:
        case shader_variable_type::INT:
        case shader_variable_type::INT2:
        case shader_variable_type::INT3:
        case shader_variable_type::INT4:
            glEnableVertexAttribArray(element.Index);
            glVertexAttribIPointer(element.Index,
                                   render_context::GetShaderVariableComponentsCount(element.Type),
                                   GetGLShaderVariableType(element.Type),
                                   layout.GetSize(),
                                   (const void *)(u64)element.Offset);
            break;
        case shader_variable_type::FLOAT:
        case shader_variable_type::FLOAT2:
        case shader_variable_type::FLOAT3:
        case shader_variable_type::FLOAT4:
            glEnableVertexAttribArray(element.Index);
            glVertexAttribPointer(element.Index,
                                  render_context::GetShaderVariableComponentsCount(element.Type),
                                  GetGLShaderVariableType(element.Type),
                                  GL_FALSE,
                                  layout.GetSize(),
                                  (const void *)(u64)element.Offset);
            break;
        case shader_variable_type::MATR4:
        {
            u32 count = render_context::GetShaderVariableComponentsCount(element.Type);
            for (uint8_t i = 0; i < count; i++)
            {
                glEnableVertexAttribArray(element.Index);
                glVertexAttribPointer(element.Index,
                                      count,
                                      GetGLShaderVariableType(element.Type),
                                      GL_FALSE,
                                      layout.GetSize(),
                                      (const void *)(element.Offset + sizeof(float) * count * i));
                glVertexAttribDivisor(element.Index, 1);
            }
            break;
        }
        default:
            SCL_CORE_ASSERT(0, "Unknown element type!");
            break;
        }
    }

    VertexBuffer->Unbind();
    this->Unbind();
}

void scl::gl_vertex_array::SetIndexBuffer(const shared<index_buffer> &IndexBuffer)
{
    this->IndexBuffer = IndexBuffer;
    this->Bind();
    IndexBuffer->Bind();
    this->Unbind();
    IndexBuffer->Unbind();
}

scl::gl_vertex_array::gl_vertex_array(mesh_type MeshType) :
    vertex_array(MeshType)
{
    glCreateVertexArrays(1, &Id);
    SCL_CORE_ASSERT(Id != 0, "Vertex array OpenGL primitive creation error.");
    SCL_CORE_SUCCES("OpenGL Vertex array with id {} created.", Id);
}

scl::gl_vertex_array::~gl_vertex_array()
{
    glDeleteVertexArrays(1, &Id);
    SCL_CORE_INFO("OpenGL Vertex array with id {} freed.", Id);
}

void scl::gl_vertex_array::Bind() const
{
    glBindVertexArray(Id);
}

void scl::gl_vertex_array::Unbind() const
{
    glBindVertexArray(0);
}
