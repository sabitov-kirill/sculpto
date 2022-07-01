/*****************************************************************//**
 * \file   gl_frame_buffer.h
 * \brief  OpenGL frame buffer class definition class.
 * 
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#pragma once

#include "gl.h"
#include "core/render/primitives/frame_buffer.h"

namespace scl
{
    /* OpenGL frame buffer class. */
    class gl_frame_buffer : public frame_buffer
    {
    private:
        frame_buffer_props Props;
        GLuint Id;
        GLuint ColorAttachmentId;
        GLuint DepthAttachmentId;

    public: /* Frame buffer getter/setter functions. */
        /* Frame buffer properties setter function. */
        void SetFrameBufferProps(const frame_buffer_props &Props);
        /* Frame buffer properties getter function. */
        const frame_buffer_props &GetFrameBufferProps() const;
        /* Api specific frame buffer collor atachment id getter function. */
        u32 GetColorAttachmentInnerId() const;

    private:
        /**
         * Recreate frame buffer according to new frame buffer properties function.
         *
         * \param None.
         * \return None.
         */
        void Invalidate();

    public:
        /**
         * OpenGL frame buffer constructor.
         * 
         * \param Props - frame buffer properties.
         */
        gl_frame_buffer(const frame_buffer_props &Props);

        /* OpenGL frame buffer default destructor. */
        ~gl_frame_buffer();

        /**
         * Bind frame buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() override;

        /**
         * Unbind frame buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() override;

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
