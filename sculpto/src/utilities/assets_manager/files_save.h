/*****************************************************************//**
 * \file   file.h
 * \brief  Assets manager file processing functions implementation module.
 *
 * \author Sabitov Kirill
 * \date   17 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl::assets_manager
{
    /**
     * File text save function.
     *
     * \param Data - file text buffer.
     * \param FilePath - outpus file path.
     * \return file text string.
     */
    void SaveFile(const std::string &Data, const std::filesystem::path &FilePath);
}