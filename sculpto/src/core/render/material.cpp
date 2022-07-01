/*****************************************************************//**
 * \file   material.h
 * \brief  Mesh material class deinition module.
 *
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "material.h"
#include "render_context.h"
#include "primitives/shader.h"
#include "primitives/texture.h"
#include "primitives/buffer.h"

const scl::shared<scl::shader_program> &scl::material::GetShader() const
{
    return Shader;
}

const scl::shared<scl::texture_2d> &scl::material::GetSurfaceTexture() const
{
    return SurfaceTexture;
}

void scl::material::SetShader(shared<shader_program> Shader)
{
    this->Shader = Shader;
}

void scl::material::SetSurfaceTexture(shared<texture_2d> SurfaceTexture)
{
    this->SurfaceTexture = SurfaceTexture;
}

void scl::material::SetMeterialData(void *Data, size_t DataSize)
{
    this->Data = Data;
    this->DataSize = DataSize;
}

scl::material::material(shared<shader_program> Shader) :
    Shader(Shader) {}

scl::material::material(shared<shader_program> Shader, void *Data, size_t DataSize) :
    Shader(Shader),
    DataBuffer(constant_buffer::Create(render_context::MATERIAL_DATA_BINDING_POINT, Data, DataSize)) {}

scl::material::material(shared<shader_program> Shader, void *Data, size_t DataSize, shared<texture_2d> SurfaceTexture) :
    Shader(Shader),
    DataBuffer(constant_buffer::Create(render_context::MATERIAL_DATA_BINDING_POINT, Data, DataSize)),
    SurfaceTexture(SurfaceTexture) {}

scl::material::material(shared<shader_program> Shader, void *Data, size_t DataSize, const image &SurfaceTextureImage) :
    Shader(Shader),
    DataBuffer(constant_buffer::Create(render_context::MATERIAL_DATA_BINDING_POINT, Data, DataSize)),
    SurfaceTexture(texture_2d::Create(SurfaceTextureImage)) {}

scl::material::material(shared<shader_program> Shader, void *Data, size_t DataSize, const std::string &SurfaceTextureFileName) :
    Shader(Shader),
    DataBuffer(constant_buffer::Create(render_context::MATERIAL_DATA_BINDING_POINT, Data, DataSize)),
    SurfaceTexture(texture_2d::Create(SurfaceTextureFileName)) {}

void scl::material::Bind() const
{
    if (Shader != nullptr) Shader->Bind();
    if (DataBuffer != nullptr && Data != nullptr && DataSize != 0) DataBuffer->Bind();
    if (SurfaceTexture != nullptr) SurfaceTexture->Bind(render_context::MATERIAL_SURFACE_TEXTURE_SLOT);
}

void scl::material::Unbind() const
{
    if (Shader != nullptr) Shader->Unbind();
    if (DataBuffer != nullptr) DataBuffer->Unbind();
    if (SurfaceTexture != nullptr) SurfaceTexture->Unbind();
}
