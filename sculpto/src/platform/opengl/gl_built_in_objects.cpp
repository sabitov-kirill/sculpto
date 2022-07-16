/*****************************************************************//**
 * \file   gl_built_in_objects.cpp
 * \brief  Sculpto library OpenGL specific rendering objects getting functions implementation module.
 * 
 * \author Sabitov Kirill
 * \date   10 July 2022
 *********************************************************************/

#include "sclpch.h"

#include "gl.h"
#include "core/resources/mesh.h"
#include "core/render/primitives/shader.h"
#include "utilities/assets_manager/shaders_load.h"

scl::shared<scl::shader_program> scl::gl::single_color_material_shader {};
scl::shared<scl::shader_program> scl::gl::phong_geometry_shader {};
scl::shared<scl::shader_program> scl::gl::phong_lighting_shader {};
scl::shared<scl::shader_program> scl::gl::shadow_pass_shader {};
scl::shared<scl::shader_program> scl::gl::tone_mapping_pass_shader {};
scl::shared<scl::shader_program> scl::gl::gaussian_blur_pass_shader {};
scl::shared<scl::shader_program> scl::gl::texture_add_pass_shader {};

scl::shared<scl::shader_program> scl::gl::GetSingleColorMaterialShader() const
{
    if (single_color_material_shader == nullptr)
        single_color_material_shader = assets_manager::LoadShader("assets/shaders/lib/single_color_geometry.glsl");
    return single_color_material_shader;
}

scl::shared<scl::shader_program> scl::gl::GetPhongGeometryShader() const
{
    if (phong_geometry_shader == nullptr)
        phong_geometry_shader = assets_manager::LoadShader("assets/shaders/lib/phong_geometry_pass.glsl");
    return phong_geometry_shader;
}

scl::shared<scl::shader_program> scl::gl::GetPhongLightingShader() const
{
    if (phong_lighting_shader == nullptr)
        phong_lighting_shader = assets_manager::LoadShader("assets/shaders/lib/phong_lighting_pass.glsl");
    return phong_lighting_shader;
}

scl::shared<scl::shader_program> scl::gl::GetShadowPassShader() const
{
    if (shadow_pass_shader == nullptr)
        shadow_pass_shader = assets_manager::LoadShader("assets/shaders/lib/shadow_pass.glsl");
    return shadow_pass_shader;
}

scl::shared<scl::shader_program> scl::gl::GetToneMappingPassShader() const
{
    if (tone_mapping_pass_shader == nullptr)
        tone_mapping_pass_shader = assets_manager::LoadShader("assets/shaders/lib/tone_mapping_pass.glsl");
    return tone_mapping_pass_shader;
}

scl::shared<scl::shader_program> scl::gl::GetGaussianBlurPassShader() const
{
    if (gaussian_blur_pass_shader == nullptr)
        gaussian_blur_pass_shader = assets_manager::LoadShader("assets/shaders/lib/gaussian_blur_pass.glsl");
    return gaussian_blur_pass_shader;
}

scl::shared<scl::shader_program> scl::gl::GetTextureAddPassShader() const
{
    if (texture_add_pass_shader == nullptr)
        texture_add_pass_shader = assets_manager::LoadShader("assets/shaders/lib/texture_add.glsl");
    return texture_add_pass_shader;
}
