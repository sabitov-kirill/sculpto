/*****************************************************************//**
 * \file   textures_load.h
 * \brief  Assets manager texture load function defintion modulule.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl { class texture_2d; }

namespace scl::assets_manager
{
    /**
     * Texture load from file function.
     * 
     * \param TextureImageFilePath - texture image file path.
     * \return loaded texture pointer.
     */
    shared<texture_2d> LoadTexture(const std::filesystem::path &TextureImageFilePath);
}
