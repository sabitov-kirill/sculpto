/*****************************************************************//**
 * \file   gl_frame_buffer.h
 * \brief  OpenGL frame buffer class definition class.
 * 
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#pragma once

#include "gl.h"
#include "gl_texture.h"
#include "core/render/primitives/frame_buffer.h"

namespace scl
{
    /* OpenGL frame buffer class. */
    class gl_frame_buffer : public frame_buffer
    {
    private:
        frame_buffer_props Props;
        GLuint Id;
        std::vector<shared<texture_2d>> ColorAttachments;
        shared<texture_2d> DepthAttachment;
        u32 ClearConfig;

    public: /* Frame buffer getter/setter functions. */
        /* Frame buffer properties setter function. */
        void SetFrameBufferProps(const frame_buffer_props &Props);
        /* Frame buffer properties getter function. */
        const frame_buffer_props &GetFrameBufferProps() const;
        /* Frame buffer color attachment getter function. */
        const shared<texture_2d> &GetColorAttachment(int Index) const override;
        /* Frame buffer depth attachment getter function. */
        const shared<texture_2d> &GetDepthAttachment(int Index) const override;

    private:
        /**
         * Recreate frame buffer according to new frame buffer properties function.
         *
         * \param None.
         * \return None.
         */
        void Invalidate();

    public:
        /* Backend api render primitive hadnle getter function. */
        render_primitive::handle GetHandle() const override { return Id; }

        /**
         * OpenGL frame buffer constructor.
         * 
         * \param Props - frame buffer properties.
         */
        gl_frame_buffer(const frame_buffer_props &Props);

        /* OpenGL frame buffer default destructor. */
        ~gl_frame_buffer() override;

        /**
         * Bind frame buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override;

        /**
         * Unbind frame buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;

        /**
         * Resize frame buffer (resize also could be called by setting frame buffer props).
         *
         * \param Width - new frame buffer width.
         * \param Height - new frame buffer height.
         * \return None.
         */
        void Resize(int Width, int Height) override;

        /**
         * Unload frame buffer render target texture from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        void Free() override;

        /**
         * Clear frame buffer funcrion.
         *
         * \param None.
         * \return None.
         */
        void Clear() override;
    };
}
