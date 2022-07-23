/*!****************************************************************//*!*
 * \file   keyboard_event.h
 * \brief  Keyboard action (key pressed, key hold, key up) event implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#pragma once

#include "../application/input_keycodes.h"
#include "utilities/event/event.h"

namespace scl
{
    /*! Keyboard action event class. */
    class keyboard_event : public event
    {
        SCL_MAKE_EVENT(Keyboard);

    private: /*! Event data. */
        bool Pressed;
        bool PrevPressed;
        bool Clicked;
        keycode KeyCode;

    public:
        /*!*
         * Keyboard event default constructor.
         * 
         * \param Pressed - is key pressed or released flag.
         * \param RepeatCount - is key pressed continiaously flag.
         * \param KeyCode - kode of acting key
         */
        keyboard_event(bool Pressed, bool PrevPressed, keycode KeyCode) :
            Pressed(Pressed), PrevPressed(PrevPressed), KeyCode(KeyCode),
            Clicked(!PrevPressed && Pressed) {}

        /*! Key pressed or released flag getter function. */
        bool GetPressed() { return Pressed; }
        /*! Key pressed continiaously flag getter function.*/
        int GetPrevPressed() { return PrevPressed; }
        /*! Key clicked once flag getter function.*/
        int GetClicked() { return Clicked; }
        /*! Kode of acting key getter function. */
        keycode GetKeyCode() { return KeyCode;  }

        /*!*
         * Convert event to string (for debug).
         *
         * \param None.
         * \return string representation of event.
         */
        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": Pressed? - " << Pressed << ", Prev. Pressed? - " << PrevPressed
               << ", Clicked? - " << Clicked << ", KeyCode - " << (int)KeyCode;
            return ss.str();
        }
    };
}
