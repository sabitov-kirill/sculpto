/*****************************************************************//**
 * \file   file.h
 * \brief  Assets manager file processing functions implementation module.
 *
 * \author Sabitov Kirill
 * \date   17 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "files_save.h"

void scl::assets_manager::SaveFile(const std::string &Data, const std::filesystem::path &FilePath)
{
    std::ofstream file(FilePath);
    SCL_CORE_ASSERT(file.is_open(), "Error during opening/creating file \"{}\".", FilePath.string());

    file << Data;
}
