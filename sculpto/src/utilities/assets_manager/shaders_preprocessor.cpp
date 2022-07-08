/*****************************************************************//**
 * \file   shader_preprocessor.cpp
 * \brief  Assets manager shaders preprocessor class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   07 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "shaders_preprocessor.h"
#include "files_load.h"

std::string scl::assets_manager::shader_preprocessor::ReadWord(std::string::const_iterator Begin,
                                                               std::string::const_iterator End,
                                                               int *SpacesCount)
{
    // skip spaces
    if (SpacesCount) *SpacesCount = 0;
    while (Begin != End && ((*Begin) == ' ' || (*Begin) == '\n'))
    {
        ++Begin;
        if (SpacesCount) ++(*SpacesCount);
    }

    // return sequence of letters from start to neares space or end of string
    return std::string(Begin, Begin + std::min(std::string(Begin, End).find(' '), std::string(Begin, End).find('\n')));
}

scl::shader_type scl::assets_manager::shader_preprocessor::ShaderTypeFromString(const std::string &ShaderTypeString)
{
    std::string shader_string_prepared(ShaderTypeString);
    std::transform(shader_string_prepared.begin(), shader_string_prepared.end(), shader_string_prepared.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    switch (StringId(shader_string_prepared.c_str()))
    {
    case "compute"_id:    return shader_type::COMPUTE;
    case "vertex"_id:     return shader_type::VERTEX;
    case "geometry"_id:   return shader_type::GEOMETRY;
    case "pixel"_id:
    case "fragment"_id:   return shader_type::PIXEL;
    }

    SCL_CORE_ASSERT(0, "Unknown shader type!");
    return shader_type();
}

void scl::assets_manager::shader_preprocessor::SeparateShaders(const std::string &ShaderDebugName,
                                                               const std::string &ShaderText,
                                                               std::vector<shader_props> &Out)
{
    size_t offset = ShaderText.find(shader_preprocessor::LexemBlockStart, 0);
    if (offset == std::string::npos)
    {
        SCL_CORE_ERROR("Error while preprocessing shader \"{}\".\nNo shader blcoks was found in text.", ShaderDebugName);
        return;
    }
    std::string global_block(ShaderText.begin(), ShaderText.begin() + offset);

    while (offset != std::string::npos)
    {
        size_t block_start_offset = ShaderText.find(' ', offset);
        if (block_start_offset == std::string::npos)
            SCL_CORE_ERROR("Error while preprocessing shader \"{}\".\nNo block text found, block skiped.", ShaderDebugName);

        size_t block_end_offset = ShaderText.find(shader_preprocessor::LexemBlockEnd, offset);
        if (block_end_offset == std::string::npos)
            SCL_CORE_ERROR("Error while preprocessing shader \"{}\".\nNo block end lexem found. Block skiped.", ShaderDebugName);

        if (block_end_offset != std::string::npos && block_start_offset != std::string::npos)
        {
            int spaces_count;
            std::string block_text(ShaderText.begin() + block_start_offset, ShaderText.begin() + block_end_offset);
            std::string block_shader_type_string = ReadWord(block_text.begin(), block_text.end(), &spaces_count);

            shader_type block_shader_type;
            bool block_shader_type_ok = true;
            std::transform(block_shader_type_string.begin(), block_shader_type_string.end(), block_shader_type_string.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            switch (StringId(block_shader_type_string.c_str()))
            {
            case "compute"_id:    block_shader_type = shader_type::COMPUTE;  break;
            case "vert"_id:
            case "vertex"_id:     block_shader_type = shader_type::VERTEX;   break;
            case "geom"_id:
            case "geometry"_id:   block_shader_type = shader_type::GEOMETRY; break;
            case "pixel"_id:
            case "frag"_id:
            case "fragment"_id:   block_shader_type = shader_type::PIXEL;    break;
            default:
                SCL_CORE_ERROR("Error while preprocessing shader \"{}\".\nShader type \"{}\" is unknown. Block skipped.", ShaderDebugName, block_shader_type_string);
                block_shader_type_ok = false;
            }

            if (block_shader_type_ok)
                Out.push_back({
                    block_shader_type,
                    global_block + std::string(block_text.begin() + spaces_count + block_shader_type_string.size(), block_text.end())
                });
        }
        offset = ShaderText.find(shader_preprocessor::LexemBlockStart, block_end_offset);
    }
}

void scl::assets_manager::shader_preprocessor::ProcessIncludes(const std::string &ShaderDebugName,
                                                               const std::string &ShaderFolderPath,
                                                               std::string &ShaderText)
{
    size_t offset;
    while ((offset = ShaderText.find(shader_preprocessor::LexemInclude, 0)) != std::string::npos)
    {
        ShaderText.erase(offset, strlen(shader_preprocessor::LexemInclude));

        int spaces_count = 0;
        std::string include_path = ReadWord(ShaderText.begin() + offset, ShaderText.end(), &spaces_count);
        ShaderText.erase(offset, spaces_count + include_path.size());

        std::string file_name = std::string(include_path.begin() + 1, include_path.end() - 1);
        std::filesystem::path file_path = ShaderFolderPath / std::filesystem::path(file_name);
        ShaderText.insert(offset, LoadFile(file_path));
    }
}
