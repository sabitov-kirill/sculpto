#include "sclpch.h"
#include "gl_texture.h"

void scl::gl_texture_2d::Create(const image &Image)
{
    SCL_CORE_ASSERT(!Image.IsEmpty(), "Trying to create texture from empty image!");

    // Generate texture primitive
    glGenTextures(1, &Id);
    SCL_CORE_ASSERT(Id != 0, "Error in creation OpenGL texture primitive.");
    glBindTexture(GL_TEXTURE_2D, Id);

    // Create texture pixels storage
    int w = Image.GetWidth(), h = Image.GetHeight();
    int c = Image.GetComponentsCount();
    GLenum color_mode = c == 3 ? GL_RGB8 : c == 4 ? GL_RGBA8 : GL_R8;
    GLenum data_format = c == 3 ? GL_RGB : c == 4 ? GL_RGBA : GL_RED;
    glTexStorage2D(GL_TEXTURE_2D, 1, color_mode, w, h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, data_format, GL_UNSIGNED_BYTE, Image.GetRawData());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Configure texture sampling.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

scl::gl_texture_2d::gl_texture_2d(const image &Image)
{
    this->Create(Image);
}

scl::gl_texture_2d::gl_texture_2d(const std::string &FileName)
{
    image texture_image(FileName);
    if (!texture_image.IsEmpty())
    {
        texture_image.FlipHorizontaly();
        this->Create(texture_image);
    }
}

void scl::gl_texture_2d::Bind(int Slot)
{
    if (Id == 0) return;

    this->Slot = Slot;
    glActiveTexture(GL_TEXTURE0 + Slot);
    glBindTexture(GL_TEXTURE_2D, Id);
}

void scl::gl_texture_2d::Unbind()
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
