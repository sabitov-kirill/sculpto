/*****************************************************************//**
 * \file   gl_texture.cpp
 * \brief  OpenGL texture class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "gl_texture.h"

void scl::gl_texture_2d::CreateColor(const image &Image, bool IsFloatingPoint)
{
    // Generate texture primitive
    glGenTextures(1, &Id);
    SCL_CORE_ASSERT(Id != 0, "Error in creation OpenGL texture primitive.");
    glBindTexture(GL_TEXTURE_2D, Id);

    // Create texture pixels storage
    int w = Image.GetWidth(), h = Image.GetHeight();
    int c = Image.GetComponentsCount();
    GLenum internal_format = IsFloatingPoint ?
        (c == 3 ? GL_RGB16F : c == 4 ? GL_RGBA16F : GL_R16F) :
        (c == 3 ? GL_RGB8   : c == 4 ? GL_RGBA8   : GL_R8  );
    GLenum format = c == 3 ? GL_RGB : c == 4 ? GL_RGBA : GL_RED;
    GLenum type = IsFloatingPoint ? GL_FLOAT : GL_UNSIGNED_BYTE;
    glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, w, h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, format, type, Image.GetRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Configure texture sampling.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void scl::gl_texture_2d::CreateDepth(const image &Image)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &Id);
    glBindTexture(GL_TEXTURE_2D, Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Image.GetWidth(), Image.GetHeight(),
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

scl::gl_texture_2d::gl_texture_2d(const image &Image, texture_type Type)
{
    switch (Type)
    {
    case scl::texture_type::COLOR:                this->CreateColor(Image, false); return;
    case scl::texture_type::COLOR_FLOATING_POINT: this->CreateColor(Image, true);  return;
    case scl::texture_type::DEPTH:                this->CreateDepth(Image);        return;
    }

    SCL_CORE_ASSERT(0, "Unknown texture type.");
}

void scl::gl_texture_2d::Bind(u32 Slot) const
{
    if (Id == 0) return;

    this->Slot = Slot;
    glActiveTexture(GL_TEXTURE0 + Slot);
    glBindTexture(GL_TEXTURE_2D, Id);
}

void scl::gl_texture_2d::Unbind() const
{
    glActiveTexture(GL_TEXTURE0 + this->Slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void scl::gl_texture_2d::Free()
{
    if (Id == 0) return;

    glDeleteTextures(0, &Id);
    Id = 0;
}

scl::image scl::gl_texture_2d::GetImage()
{
    // TODO: Load image raw data from GPU and create image container from it.
    return image();
}
