/*!****************************************************************//*!*
 * \file   vieport_events.h
 * \brief  Viewport resize event implementation module.
 * 
 * \author Sabitov Kirill
 * \date   13 July 2022
 *********************************************************************/

#pragma once

#include "utilities/event/event.h"

namespace scl
{
    /*! Application viewports resize event class. */
    class viewport_resize_event: public event
    {
        SCL_MAKE_EVENT(ViewportResize);

    private: /*! Application viewport resize evet data. */
        u32 Width, Height;
        int ViewportId;

    public:
        /*!*
         * Window resize vent constructor.
         *
         * \param Width - new width of window.
         * \param Height - new height of window.
         * \param ViewportId - id of resizeing viewport (zero viewport id is always window viewport).
         */
        viewport_resize_event(int Width, int Height, int ViewportId) :
            Width(Width), Height(Height), ViewportId(ViewportId) {}

        /*! New window width getter function. */
        u32 GetWidth() const { return Width; }
        /*! New window height getter function. */
        u32 GetHeight() const { return Height; }
        /*! Resizing viewport id getter function. */
        int GetViewportId() const { return ViewportId; }

        /*!*
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
