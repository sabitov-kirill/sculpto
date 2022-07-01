/*****************************************************************//**
 * \file   frame_buffer.h
 * \brief  Frame buffer interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Frame buffer enum. */
    enum class frame_buffer_format
    {
    };

    /* Frame buffer specification structure. */
    struct frame_buffer_props
    {
        // frame_buffer_format Format;     /* Frame buffer texture format. */
        int Width, Height;              /* Frame buffer texture size */
        int Samples = 1;                /* Frame buffer samples count. */
        bool SwapChainTarget = false;   /* Frame buffer creating in purpose of rendering to screen. */
    };

    /* Frame buffer interface. */
    class frame_buffer
    {
    public: /* Frame buffer getter/setter functions. */
        /* Frame buffer properties setter function. */
        virtual void SetFrameBufferProps(const frame_buffer_props &Props) = 0;
        /* Frame buffer properties getter function. */
        virtual const frame_buffer_props &GetFrameBufferProps() const = 0;
        /* Api specific frame buffer collor atachment id getter function. */
        virtual u32 GetColorAttachmentInnerId() const = 0;

    public:
        /**
         * Bind frame buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() = 0;

        /**
         * Unbind frame buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() = 0;

        /**
         * Unload frame buffer render target texture from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /**
         * Create frame buffer function.
         * 
         * \props Props - propsrties (specification) of creating frame buffer.
         * \return None.
         */
        static shared<frame_buffer> Create(const frame_buffer_props &Props);
    };
}
