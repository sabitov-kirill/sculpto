/*****************************************************************//**
 * \file   gl_built_in_objects.cpp
 * \brief  Sculpto library OpenGL specific rendering objects getting functions implementation module.
 * 
 * \author Sabitov Kirill
 * \date   10 July 2022
 *********************************************************************/

#include "sclpch.h"

#include "gl.h"
#include "core/render/primitives/shader.h"
#include "core/resources/mesh.h"
#include "utilities/assets_manager/shaders_load.h"

scl::shared<scl::shader_program> scl::gl::GetSingleColorMaterialShader() const
{
    static shared<shader_program> single_color_material_shader = nullptr;
    if (single_color_material_shader == nullptr)
        single_color_material_shader = assets_manager::LoadShader("assets/shaders/single_color.glsl");
    return single_color_material_shader;
}

scl::shared<scl::shader_program> scl::gl::GetPhongMaterialShader() const
{
    static shared<shader_program> phong_material_shader = nullptr;
    if (phong_material_shader == nullptr)
        phong_material_shader = assets_manager::LoadShader("assets/shaders/phong.glsl");
    return phong_material_shader;
}

scl::shared<scl::shader_program> scl::gl::GetShadowPassShader() const
{
    static shared<shader_program> shadow_pass_shader = nullptr;
    if (shadow_pass_shader == nullptr)
        shadow_pass_shader = assets_manager::LoadShader("assets/shaders/shadow_pass.glsl");
    return shadow_pass_shader;
}

scl::shared<scl::shader_program> scl::gl::GetToneMappingPassShader() const
{
    static shared<shader_program> tone_mapping_pass_shader = nullptr;
    if (tone_mapping_pass_shader == nullptr)
        tone_mapping_pass_shader = assets_manager::LoadShader("assets/shaders/tone_mapping_pass.glsl");
    return tone_mapping_pass_shader;
}

scl::shared<scl::shader_program> scl::gl::GetGaussianBlurPassShader() const
{
    static shared<shader_program> gaussian_blur_pass_shader = nullptr;
    if (gaussian_blur_pass_shader == nullptr)
        gaussian_blur_pass_shader = assets_manager::LoadShader("assets/shaders/gaussian_blur_pass.glsl");
    return gaussian_blur_pass_shader;
}
