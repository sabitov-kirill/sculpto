/*****************************************************************//**
 * \file   gl_texture.h
 * \brief  OpenGL texture class definition module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "gl.h"
#include "core/render/primitives/texture.h"

namespace scl
{
    /* Texture interface. */
    class gl_texture_2d : public texture_2d
    {
    private: /* OpenGL texture data. */
        GLuint Id {};
        int Slot {};

    public: /* OpenGL texture getter/setter functions. */
        /* Texture slot getter function.
         * Set during texture binding.
         */
        int GetSlot() { return Slot; }

    private:
        /**
         * Create OpenGL texture from image container function.
         * 
         * \param Image - image container to create texture from.
         * \return None.
         */
        void Create(const image &Image);

    public:
        /**
         * OpenGL texture constructor by image container.
         *
         * \param Image - image container to get data from.
         * \return created texture pointer.
         */
        gl_texture_2d(const image &Image);

        /**
         * OpenGL texture constructor by image loaded from specified file.
         *
         * \param FileName - file name to load image from.
         * \return pointer to created texture.
         */
        gl_texture_2d(const std::string &FileName);

        /**
         * Bind texture to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind(int Slot) override;

        /**
         * Unbind texture from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() override;

        /**
         * Unload texture from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        void Free() override;

        /**
         * Load texture image from GPU memory function.
         *
         * \param None.
         * \return texture image.
         */
        image GetImage() override;
    };
}
