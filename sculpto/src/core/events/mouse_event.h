/*****************************************************************//**
 * \file   mouse_event.h
 * \brief  Mouse actions events (buttons click, move, wheel) events implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#pragma once

#include "utilities/event/event.h"

namespace scl
{
    enum class mouse_button
    {
#ifdef SCL_PLATFORM_WINDOWS
        NONE = 0,
        LEFT = 1,
        RIGHT = 2,
        MIDDLE = 16,
#else
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
#endif
    };

    /* Mouse buttons click events class. */
    class mouse_button_event: public event
    {
        SCL_MAKE_EVENT(MouseButton);

    private: /* Mouse button click data. */
        bool Pressed;
        int X, Y;
        mouse_button KeyCode;

    public:
        /**
         * Mouse button click default constructor..
         * 
         * \param Pressed - is mouse key pressed or released flag.
         * \param DbClick - is button pressecd second time in a row flag.
         * \param X - mouse x position.
         * \param Y - mouse y possition.
         * \param KeyCode - mouse button code.
         */
        mouse_button_event(bool Pressed, int X, int Y, mouse_button KeyCode) :
            Pressed(Pressed), X(X), Y(Y), KeyCode(KeyCode) {}

        /* Mouse key pressed or released flag getter function. */
        bool GetPressed() { return Pressed; }
        /* Mouse x position getter function. */
        int GetX() { return X; }
        /* Mouse y position getter function. */
        int GetY() { return Y; }
        /* Mouse button code getter function. */
        mouse_button GetKeyCode() { return KeyCode; }

        /**
         * Convert event to string (for debug).
         *
         * \param None.
         * \return string representation of event.
         */
        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": Pressed? - " << Pressed
               << ", (X, Y) - (" << X << ", " << Y << "), KeyCode - " << (int)KeyCode;
            return ss.str();
        }
    };

    /* Mouse wheel scroll event class. */
    class mouse_wheel_event: public event
    {
        SCL_MAKE_EVENT(MouseWheel);

    private :/* Mouse wheel data. */
        int WheelPosDelta;

    public:
        /**
         * Mouse wheel scroll event default constructor.
         * 
         * \param WheelPos - mouse wheel position delta.
         */
        mouse_wheel_event(int WheelPosDelta) :
            WheelPosDelta(WheelPosDelta) {}

        /* Mouse wheel position delta getter function. */
        int GetWheelPos() { return WheelPosDelta; }

        /**
         * Convert event to string (for debug).
         *
         * \param None.
         * \return string representation of event.
         */
        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": Wheel position delta - " << WheelPosDelta;
            return ss.str();
        }
    };

    /* Mouse move event class. */
    class mouse_move_event: public event
    {
        SCL_MAKE_EVENT(MouseMove);

    private: /* Mouse move data. */
        int X, Y;
        bool RButton, LButton, MButton;
        bool Shift, Control;

    public:
        /**
         * Mouse move event deault constructor..
         * 
         * \param X - mouse x possition.
         * \param Y - mouse y possition.
         * \param RButton - mouse right button state.
         * \param LButton - mouse left button state.
         * \param MButton - mouse middle button state.
         * \param Shift - shift key state.
         * \param Control - contrl key state.
         */
        mouse_move_event(int X, int Y, bool RButton, bool LButton, bool MButton,
                         bool Shift, bool Control) :
            X(X), Y(Y), RButton(RButton), LButton(LButton), MButton(MButton),
            Shift(Shift), Control(Control) {}

        /* Mouse x position getter function. */
        int GetX() { return X; }
        /* Mouse y position getter function. */
        int GetY() { return Y; }
        /* Mouse right button state getter function. */
        bool GetRButton() { return RButton; }
        /* Mouse left button state getter function. */
        bool GetLButton() { return LButton; }
        /* Mouse middle button state getter function. */
        bool GetMButton() { return MButton; }
        /* Shift key state getter function. */
        bool GetShift() { return Shift; }
        /* Constrol state getter function. */
        bool GetControl() { return Control; }

        /**
         * Convert event to string (for debug).
         *
         * \param None.
         * \return string representation of event.
         */
        inline std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": (X, Y) - (" << X << ", " << Y
               << "), RButton - " << RButton << ", LButton - " << LButton << ", MButton - " << MButton;
            return ss.str();
        }
    };
}
