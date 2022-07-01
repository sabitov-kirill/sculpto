/*****************************************************************//**
 * \file   shader.cpp
 * \brief  Shader program interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "../render_context.h"
#include "shader.h"
#include "platform/opengl/gl_shader.h"

scl::shader_program::shader_program(const std::initializer_list<shader_props> &Shaders, const std::string &DebugName) :
    Shaders(Shaders), DebugName(DebugName) {}

scl::shared<scl::shader_program> scl::shader_program::Create(const std::initializer_list<shader_props> &Shaders, const std::string &DebugName)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<gl_shader_program>(Shaders, DebugName);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}

scl::shared<scl::shader_program> scl::shader_program::Create(const std::string &FolderPath)
{
    // TODO: Scan folder and load all available shaders.

    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
