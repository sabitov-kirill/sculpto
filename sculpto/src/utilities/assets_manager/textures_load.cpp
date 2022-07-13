/*****************************************************************//**
 * \file   textures_load.cpp
 * \brief  Assets manager texture load function defintion modulule.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "textures_load.h"
#include "core/render/render_context.h"
#include "core/render/primitives/texture.h"

scl::shared<scl::texture_2d> scl::assets_manager::LoadTexture(const std::filesystem::path &TextureImageFilePath)
{
    image texture_image(TextureImageFilePath.string());
    if (texture_image.IsEmpty())
    {
        SCL_CORE_ERROR("Texture \"{}\" not found!", TextureImageFilePath.string());
        return nullptr;
    }

    if (render_context::GetApi() == render_context_api::OpenGL)
        texture_image.FlipHorizontaly();

    return texture_2d::Create(texture_image, texture_type::COLOR);
}
