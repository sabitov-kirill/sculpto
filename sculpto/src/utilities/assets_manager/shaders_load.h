/*****************************************************************//**
 * \file   shaders_loader.h
 * \brief  Assets manager shader load function defintion modulule.
 * 
 * \author Sabitov Kirill
 * \date   07 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

/* Shader program class declaration. */
namespace scl { class shader_program; }

namespace scl::assets_manager
{
    /**
     * Load shader from file function.
     * 
     * \param ShaderProgamFilePath - file containing all shaders to compile in program.
     * \return pointer to shader, loaded from file.
     */
    shared<shader_program> LoadShader(const std::filesystem::path &ShaderProgamFilePath);

    /**
     * Load shader from file function.
     * 
     * \param VertexShaderFilePath - file containing vertex shader.
     * \param PixelShaderFilePath - file containing pixel(fragment) shader.
     * \return pointer to shader, loaded from files.
     */
    shared<shader_program> LoadShader(const std::filesystem::path &VertexShaderFilePath,
                                      const std::filesystem::path &PixelShaderFilePath);

    /**
     * Load shader from file function.
     *
     * \param VertexShaderFilePath - file containing vertex shader.
     * \param GeomShaderFilePath - file containing pixel(fragment) shader.
     * \param PixelShaderFilePath - file containing pixel(fragment) shader.
     * \return pointer to shader, loaded from files.
     */
    shared<shader_program> LoadShader(const std::filesystem::path &VertexShaderFilePath,
                                      const std::filesystem::path &GeomShaderFilePath,
                                      const std::filesystem::path &PixelShaderFilePath);

    /**
     * Update existing shader program function.
     * 
     * \param ShaderProgram - shader program to update.
     * \return None.
     */
    void UpdateShader(shared<shader_program> &ShaderProgram);
}
