/*****************************************************************//**
 * \file   texture.h
 * \brief  Texture interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "render_primitive.h"
#include "utilities/image/image.h"

namespace scl
{
    /* Texture types enum class. */
    enum class texture_2d_type
    {
        COLOR,   /* Default color texture type. */
        DEPTH,   /* Depth buffer texture type. */
        STENCIL, /* ? Stencil buffer texture type. Probably worse than buffer. */
    };

    /* Texture interface. */
    class texture_2d : public render_primitive
    {
    public:
        /**
         * Bind texture to current render stage function.
         *
         * \param Slot - texture slot to bind it in.
         * \return None.
         */
        virtual void Bind(u32 Slot) const = 0;

        /**
         * Unbind texture from current render stage function.
         * 
         * \param Slot - texture slot to unbind it from.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /**
         * Load texture image from GPU memory function.
         * 
         * \param None.
         * \return texture image.
         */
        virtual image GetImage() = 0;

        /**
         * Unload texture from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /**
         * Create texture from image container function.
         *
         * \param Image - image container to get data from.
         * \return created texture pointer.
         */
        static shared<texture_2d> Create(const image &Image, texture_2d_type Type = texture_2d_type::COLOR);

        // TODO: Delete these creation function, move functionality to assets manager
        /**
         * Create texture from loaded from specified file image function.
         * 
         * \param FileName - file name to load image from.
         * \return pointer to created texture.
         */
        static shared<texture_2d> Create(const std::string &FileName);
    };
}
