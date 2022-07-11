/*****************************************************************//**
 * \file   editor_viewport_window.h
 * \brief  Sculpto editor viewport window class definition module.
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
    class scene;

    /* Sculpto editor viewport window class. */
    class viewport_window
    {
    private: /* Sculpto editor viewport data. */
        shared<frame_buffer> ViewportBuffer {};
        bool ResizeBuffer {};
        scene *Scene {};

    public:
        /**
         * Viewport window default constructor.
         * 
         * \param ViewportBuffer - frame buffer to be displayed in viewport window.
         * \param ResizeBuffer - flag, indicating whether resize buffer on window size change or not.
         * \param Scene - scene, to call resize event in.
         */
        viewport_window(shared<frame_buffer> ViewportBuffer, bool ResizeBuffer = true, scene *Scene = nullptr);

        /**
         * Draw viewport window function.
         *
         * \param None.
         * \return None.
         */
        void Draw();
    };
}
