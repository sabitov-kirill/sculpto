/*!****************************************************************//*!*
 * \file   render_context.h
 * \brief  Base, abstract, backend render api independent render
 *         context class implementation module.
 *
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#include "sclpch.h"

#include "render_context.h"
#include "platform/opengl/gl.h"

scl::u32 scl::render_context::GetShaderVariableTypeSize(shader_variable_type Type)
{
    switch (Type)
    {
    case scl::shader_variable_type::BOOL:   return 1;
    case scl::shader_variable_type::INT:    return 4;
    case scl::shader_variable_type::INT2:   return 8;
    case scl::shader_variable_type::INT3:   return 12;
    case scl::shader_variable_type::INT4:   return 16;
    case scl::shader_variable_type::FLOAT:  return 4;
    case scl::shader_variable_type::FLOAT2: return 8;
    case scl::shader_variable_type::FLOAT3: return 12;
    case scl::shader_variable_type::FLOAT4: return 16;
    case scl::shader_variable_type::MATR3:  return 48;
    case scl::shader_variable_type::MATR4:  return 64;
    }

    SCL_CORE_ASSERT(0, "Unknown shader variable type!");
    return u32();
}

scl::u32 scl::render_context::GetShaderVariableComponentsCount(shader_variable_type Type)
{
    switch (Type)
    {
    case scl::shader_variable_type::BOOL:   return 1;
    case scl::shader_variable_type::INT:    return 1;
    case scl::shader_variable_type::INT2:   return 2;
    case scl::shader_variable_type::INT3:   return 3;
    case scl::shader_variable_type::INT4:   return 4;
    case scl::shader_variable_type::FLOAT:  return 1;
    case scl::shader_variable_type::FLOAT2: return 2;
    case scl::shader_variable_type::FLOAT3: return 3;
    case scl::shader_variable_type::FLOAT4: return 4;
    case scl::shader_variable_type::MATR3:  return 3;
    case scl::shader_variable_type::MATR4:  return 4;
    }

    SCL_CORE_ASSERT(0, "Unknown shader variable type!");
    return u32();
}

scl::render_context_api scl::render_context::Api = scl::render_context_api::OpenGL;

scl::unique<scl::render_context> scl::render_context::Create()
{
    switch (Api)
    {
        case scl::render_context_api::OpenGL:  return CreateUnique<gl>();
        case scl::render_context_api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ERROR("Unknown render API was selected.");
    return nullptr;
}
