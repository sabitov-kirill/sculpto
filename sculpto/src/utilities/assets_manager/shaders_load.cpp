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
    SCL_CORE_INFO("Shader creation from file \"{}\" started.", ShaderProgamFilePath.string());

    std::string shader_text = LoadFile(ShaderProgamFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgamFilePath.string(), ShaderProgamFilePath.parent_path().string(), shader_text);

    std::vector<shader_props> Out;
    assets_manager::shader_preprocessor::SeparateShaders(ShaderProgamFilePath.string(), shader_text, Out);

    auto shader = shader_program::Create(Out, ShaderProgamFilePath.string());
    shader->SingleSourceFileName = ShaderProgamFilePath.string();
    return shader;
}

scl::shared<scl::shader_program> scl::assets_manager::LoadShader(const std::filesystem::path &VertexShaderFilePath,
                                                                 const std::filesystem::path &PixelShaderFilePath)
{
    std::stringstream file_buffer;
    std::string shader_text;
    std::vector<shader_props> shaders;

    SCL_CORE_INFO("Shader creation from files \"{}\", \"{}\" started.", VertexShaderFilePath.string(), PixelShaderFilePath.string());

    // Vertex shader
    shader_text = LoadFile(VertexShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(VertexShaderFilePath.string(), VertexShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::VERTEX, shader_text });

    // Fragment shader
    shader_text = LoadFile(PixelShaderFilePath);
    assets_manager::shader_preprocessor::ProcessIncludes(PixelShaderFilePath.string(), PixelShaderFilePath.parent_path().string(), shader_text);
    shaders.push_back({ shader_type::PIXEL, shader_text });

    auto shader = shader_program::Create(shaders, VertexShaderFilePath.string());
    shader->VertexShadersourceFileName = VertexShaderFilePath.string();
    shader->PixelShadersourceFileName = PixelShaderFilePath.string();
    return shader;
}

scl::shared<scl::shader_program> scl::assets_manager::LoadShader(const std::filesystem::path &VertexShaderFilePath,
                                                                 const std::filesystem::path &GeomShaderFilePath,
                                                                 const std::filesystem::path &PixelShaderFilePath)
{
    std::stringstream file_buffer;
    std::string shader_text;
    std::vector<shader_props> shaders;

    SCL_CORE_INFO("Shader creation from files \"{}\", \"{}\", \"{}\" started.", VertexShaderFilePath.string(), GeomShaderFilePath.string(), PixelShaderFilePath.string());

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

    auto shader = shader_program::Create(shaders, VertexShaderFilePath.string());
    shader->VertexShadersourceFileName = VertexShaderFilePath.string();
    shader->GeometryShadersourceFileName = GeomShaderFilePath.string();
    shader->PixelShadersourceFileName = PixelShaderFilePath.string();
    return shader;
}

void scl::assets_manager::UpdateShader(shared<shader_program> ShaderProgram)
{
    std::stringstream file_buffer;
    std::string shader_text;
    std::vector<shader_props> shaders;

    if (ShaderProgram->SingleSourceFileName != "") {
        SCL_CORE_INFO("Shader updation from file \"{}\" started.", ShaderProgram->SingleSourceFileName);
        shader_text = LoadFile(ShaderProgram->SingleSourceFileName);
        assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgram->SingleSourceFileName,
                                                             std::filesystem::path(ShaderProgram->SingleSourceFileName).parent_path().string(),
                                                             shader_text);

        assets_manager::shader_preprocessor::SeparateShaders(ShaderProgram->SingleSourceFileName, shader_text, shaders);
        ShaderProgram->Update(shaders);
    } else if (ShaderProgram->VertexShadersourceFileName != "" && ShaderProgram->PixelShadersourceFileName != "" &&
               ShaderProgram->GeometryShadersourceFileName == "") {
        SCL_CORE_INFO("Shader updation from files \"{}\", \"{}\" started.", ShaderProgram->VertexShadersourceFileName, ShaderProgram->PixelShadersourceFileName);

        // Vertex shader
        shader_text = LoadFile(ShaderProgram->VertexShadersourceFileName);
        assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgram->VertexShadersourceFileName,
                                                             std::filesystem::path(ShaderProgram->VertexShadersourceFileName).parent_path().string(),
                                                             shader_text);
        shaders.push_back({ shader_type::VERTEX, shader_text });

        // Fragment shader
        shader_text = LoadFile(ShaderProgram->PixelShadersourceFileName);
        assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgram->PixelShadersourceFileName,
                                                             std::filesystem::path(ShaderProgram->PixelShadersourceFileName).parent_path().string(),
                                                             shader_text);
        shaders.push_back({ shader_type::PIXEL, shader_text });

        ShaderProgram->Update(shaders);
    } else {
        SCL_CORE_INFO("Shader creation from files \"{}\", \"{}\", \"{}\" started.", ShaderProgram->VertexShadersourceFileName, ShaderProgram->GeometryShadersourceFileName, ShaderProgram->PixelShadersourceFileName);

        // Vertex shader
        shader_text = LoadFile(ShaderProgram->VertexShadersourceFileName);
        assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgram->VertexShadersourceFileName,
                                                             std::filesystem::path(ShaderProgram->VertexShadersourceFileName).parent_path().string(),
                                                             shader_text);
        shaders.push_back({ shader_type::VERTEX, shader_text });

        // Geometry shader
        shader_text = LoadFile(ShaderProgram->GeometryShadersourceFileName);
        assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgram->GeometryShadersourceFileName,
                                                             std::filesystem::path(ShaderProgram->GeometryShadersourceFileName).parent_path().string(),
                                                             shader_text);
        shaders.push_back({ shader_type::GEOMETRY, shader_text });

        // Fragment shader
        shader_text = LoadFile(ShaderProgram->PixelShadersourceFileName);
        assets_manager::shader_preprocessor::ProcessIncludes(ShaderProgram->PixelShadersourceFileName,
                                                             std::filesystem::path(ShaderProgram->PixelShadersourceFileName).parent_path().string(),
                                                             shader_text);
        shaders.push_back({ shader_type::PIXEL, shader_text });

        ShaderProgram->Update(shaders);
    }
}
