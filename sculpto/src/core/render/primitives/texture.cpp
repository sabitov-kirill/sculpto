/*****************************************************************//**
 * \file   texture.cpp
 * \brief  Texture interface implementation module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "../render_context.h"
#include "texture.h"
#include "platform/opengl/gl_texture.h"

scl::shared<scl::texture_2d> scl::texture_2d::Create(const image &Image)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<gl_texture_2d>(Image);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}

scl::shared<scl::texture_2d> scl::texture_2d::Create(const std::string &FileName)
{
    switch (render_context::GetApi())
    {
    case scl::render_context::api::OpenGL:  return CreateShared<gl_texture_2d>(FileName);
    case scl::render_context::api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
