/*!****************************************************************//*!*
 * \file   files_load.cpp
 * \brief  Assets manager file processing functions implementation module.
 * 
 * \author Sabitov Kirill
 * \date   08 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "files_load.h"

std::string scl::assets_manager::LoadFile(const std::filesystem::path &FilePath)
{
    std::stringstream file_buffer;
    std::ifstream file(FilePath);
    SCL_CORE_ASSERT(file.is_open(), "File \"{}\" not found.", FilePath.string());

    file_buffer << file.rdbuf();
    return file_buffer.str();
}
