/*!****************************************************************//*!*
 * \file   frame_buffer.h
 * \brief  Frame buffer interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#pragma once

#include "render_primitive.h"

namespace scl
{
    /*! Texture class declaration. */
    class texture_2d;

    /*! Frame buffer specification structure. */
    struct frame_buffer_props
    {
        int  Width { 16 }, Height { 16 };   /*! Frame buffer texture size */
        int  Samples { 1 };                 /*! Frame buffer samples count. */
        bool IsSwapChainTarget {};          /*! Flag, showing wheather frame buffer creating in purpose of rendering to screen or not. */
        bool IsHDR {};                      /*! Flag, showing wheather frame buffer uses high dynamic range or default [0; 1] range. */
        int  ColorAttachmentsCount { 1 };   /*! Frame buffer color attachments count. */
        int  DepthAttachmentsCount { 1 };   /*! Frame buffer depth attachments count. */
                                            /*! Note: at least one of attachments count must be > 0. */

        /*! Frame buffer default constructor. */
        frame_buffer_props() = default;

        /*!*
         * Frame buffer filling constructor.
         * 
         * \param Width, Height - frame buffer texture size.
         * \param Samples - frame buffer samples count.
         * \param IsSwapChainTarget - frame buffer creating in purpose of rendering to screen.
         * \param ColorAttachmentsCount - frame buffer color attachments count.
         * \param DepthAttachmentsCount - frame buffer depth attachments count.
         */
        frame_buffer_props(int Width, int Height, int Samples = 1, bool IsSwapChainTarget = true, int ColorAttachmentsCount = 1, int DepthAttachmentsCount = 1, bool IsHDR = false) :
            Width(Width), Height(Height), Samples(Samples), IsSwapChainTarget(IsSwapChainTarget), IsHDR(IsHDR),
            ColorAttachmentsCount(ColorAttachmentsCount), DepthAttachmentsCount(DepthAttachmentsCount) {}
    };

    /*! Frame buffer interface. */
    class frame_buffer: public render_primitive
    {
    public: /*! Frame buffer getter/setter functions. */
        /*! Frame buffer properties setter function. */
        virtual void SetFrameBufferProps(const frame_buffer_props &Props) = 0;
        /*! Frame buffer properties getter function. */
        virtual const frame_buffer_props &GetFrameBufferProps() const = 0;
        /*! Frame buffer color attachment getter function. */
        virtual const shared<texture_2d> &GetColorAttachment(int Index = 0) const = 0;
        /*! Frame buffer depth attachment getter function. */
        virtual const shared<texture_2d> &GetDepthAttachment(int Index = 0) const = 0;

    public:
        /*! Frame buffer default deatructor. */
        virtual ~frame_buffer() = default;

        /*!*
         * Bind frame buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const = 0;

        /*!*
         * Unbind frame buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /*!*
         * Resize frame buffer (resize also could be called by setting frame buffer props).
         *
         * \param Width - new frame buffer width.
         * \param Height - new frame buffer height.
         * \return None.
         */
        virtual void Resize(int Width, int Height) = 0;

        /*!*
         * Unload frame buffer render target texture from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /*!*
         * Clear frame buffer funcrion.
         * 
         * \param None.
         * \return None.
         */
        virtual void Clear() = 0;

        /*!*
         * Create frame buffer function.
         * 
         * \props Props - propsrties (specification) of creating frame buffer.
         * \return None.
         */
        static shared<frame_buffer> Create(const frame_buffer_props &Props);
    };
}
