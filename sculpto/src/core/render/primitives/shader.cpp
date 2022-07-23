/*!****************************************************************//*!*
 * \file   shader.cpp
 * \brief  Shader program interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "shader.h"
#include "platform/opengl/gl_shader.h"

scl::shader_program::shader_program(const std::string &DebugName) :
    DebugName(DebugName) {}

scl::shared<scl::shader_program> scl::shader_program::Create(const std::vector<shader_props> &Shaders, const std::string &DebugName)
{
    switch (render_context::GetApi())
    {
    case scl::render_context_api::OpenGL:  return CreateShared<gl_shader_program>(Shaders, DebugName);
    case scl::render_context_api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
