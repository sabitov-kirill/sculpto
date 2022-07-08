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
        mutable u32 Slot {};
        GLuint Id {};

    public: /* OpenGL texture getter/setter functions. */
        /* Backend api render primitive hadnle getter function. */
        render_primitive::handle GetHandle() const override { return Id; }

    private:
        /**
         * Create OpenGL color texture from image container function.
         * 
         * \param Image - image container to create texture from.
         * \return None.
         */
        void CreateColor(const image &Image);

        /**
         * Create OpenGL depth texture function.
         * 
         * \param None.
         * \return None.
         */
        void CreateDepth(const image &Image);

    public:
        /**
         * OpenGL texture constructor by image container.
         *
         * \param Image - image container to get data from (pixels data can be null).
         * \param Type - texture type.
         * \return created texture pointer.
         */
        gl_texture_2d(const image &Image, texture_2d_type Type);

        /**
         * Bind texture to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind(u32 Slot) const override;

        /**
         * Unbind texture from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;

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
