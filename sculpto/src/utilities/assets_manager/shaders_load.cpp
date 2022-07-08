/*****************************************************************//**
 * \file   shaders_loader.h
 * \brief  Assets manager shader load function defintion modulule.
 *
 * \author Sabitov Kirill
 * \date   07 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "shaders_load.h"
#include "shaders_preprocessor.h"
#include "files_load.h"
#include "core/render/primitives/shader.h"

scl::shared<scl::shader_program> scl::assets_manager::LoadShader(const std::filesystem::path &ShaderProgamFilePath)
{
    std::string shader_text = LoadFile(ShaderProgamFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgamFilePath.string(), ShaderProgamFilePath.parent_path().string(), shader_text);

    std::vector<shader_props> Out;
    assets_manager::shader_preprocessor::SeparateShaders(ShaderProgamFilePath.string(), shader_text, Out);
    return shader_program::Create(Out, ShaderProgamFilePath.string());
}

scl::shared<scl::shader_program> scl::assets_manager::LoadShader(const std::filesystem::path &VertexShaderFilePath,
                                                                 const std::filesystem::path &PixelShaderFilePath)
{
    std::stringstream file_buffer;
    std::string shader_text;
    std::vector<shader_props> shaders;

    // Vertex shader
    shader_text = LoadFile(VertexShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(VertexShaderFilePath.string(), VertexShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::VERTEX, shader_text });

    // Fragment shader
    shader_text = LoadFile(PixelShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(PixelShaderFilePath.string(), PixelShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::PIXEL, shader_text });

    return shader_program::Create(shaders, VertexShaderFilePath.string());
}

scl::shared<scl::shader_program> scl::assets_manager::LoadShader(const std::filesystem::path &VertexShaderFilePath,
                                                                 const std::filesystem::path &GeomShaderFilePath,
                                                                 const std::filesystem::path &PixelShaderFilePath)
{
    std::stringstream file_buffer;
    std::string shader_text;
    std::vector<shader_props> shaders;

    // Vertex shader
    shader_text = LoadFile(VertexShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(VertexShaderFilePath.string(), VertexShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::VERTEX, shader_text });

    // Geometry shader
    shader_text = LoadFile(GeomShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(GeomShaderFilePath.string(), GeomShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::GEOMETRY, shader_text });

    // Fragment shader
    shader_text = LoadFile(PixelShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(PixelShaderFilePath.string(), PixelShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::PIXEL, shader_text });

    return shader_program::Create(shaders, VertexShaderFilePath.string());
}
