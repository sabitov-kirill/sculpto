/*****************************************************************//**
 * \file   window_event.h
 * \brief  Window update specific events implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#pragma once

#include "utilities/event/event.h"

namespace scl
{
    /* Window close event class. */
    class window_close_event: public event
    {
        SCL_MAKE_EVENT(WindowClose);
    };

    /* Window resize event class. */
    class window_resize_event: public event
    {
        SCL_MAKE_EVENT(WindowResize);

    private:
        u32 Width, Height;

    public:
        /**
         * Window resize vent constructor.
         * 
         * \param Width - new width of window.
         * \param Height - new height of window.
         */
        window_resize_event(int Width, int Height) :
            Width(Width), Height(Height) {}

        /* New window width getter function. */
        u32 GetWidth() const { return Width; }
        /* New window height getter function. */
        u32 GetHeight() const { return Height; }

        /**
         * Convert event to string (for debug).
         *
         * \param None.
         * \return string representation of event.
         */
        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << Width << ", " << Height;
            return ss.str();
        }
    };
}
