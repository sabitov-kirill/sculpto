/*****************************************************************//**
 * \file   file.h
 * \brief  Assets manager file processing functions implementation module.
 * 
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl::assets_manager
{
    /**
     * File texto load function.
     * 
     * \param FilePath - file to path to load.
     * \return file text string.
     */
    std::string LoadFile(const std::filesystem::path &FilePath);
}
