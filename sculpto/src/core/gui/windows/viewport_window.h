/*****************************************************************//**
 * \file   editor_viewport_window.h
 * \brief  Viewport window class definition module.
 *
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Classes declaration. */
    class frame_buffer;

    /* Viewport window class. */
    class viewport_window
    {
    private: /* Viewport window data. */
        weak<frame_buffer> ViewportBuffer;
        int    ViewportId { 30 };
        int    ViewportWidth { 16 };
        int    ViewportHeight { 16 };

    public: /* Viewport window data getter/setter functinos. */
        /* Viewport window viewport id getter function. Viewport resize event invoked with it. */
        int GetViewportId() const { return ViewportId; }
        /* Viewport window viewport width getter function. */
        int GetViewportWidth() const { return ViewportWidth; }
        /* Viewport window viewport height getter function. */
        int GetViewportHegith() const { return ViewportHeight; }

        void SetViewportBuffer(const shared<frame_buffer> &ViewportBuffer) { this->ViewportBuffer = ViewportBuffer; }
        /* Viewport window viewport id setter function. Viewport resize event invoked with it. */
        void SetViewportId(int ViewportId) {
            SCL_CORE_ASSERT(ViewportId != 0, "ViewportId = 0 is not allowed, it is reserved by main application window.");
            this->ViewportId = ViewportId;
        }

    public: /* Viewport window methods. */
        /**
         * Viewport window default constructor.
         * 
         * \param ViewportBuffer - frame buffer to be displayed in viewport window.
         * \param ViewportId - viewport window viewport id.
         */
        viewport_window(const shared<frame_buffer> &ViewportBuffer, int ViewportId = 30);

        /**
         * Draw viewport window function.
         *
         * \param None.
         * \return None.
         */
        void Draw();
    };
}
