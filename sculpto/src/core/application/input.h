/*****************************************************************//**
 * \file   input.h
 * \brief  Abstract, platform independent input system definition mpdule.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "platform_detection.h"

namespace scl
{
    struct mouse_data
    {
        int PosX { 0 };      /* Mouse x absolute screen position. */
        int PosY { 0 };      /* Mouse y absolute screen position. */
        int PosZ { 0 };      /* Mouse wheel absolute scrool value. */
        int PosDeltaX { 0 }; /* Mouse x screen position delta (relative to previus readed) value. */
        int PosDeltaY { 0 }; /* Mouse y screen position delta (relative to previus readed) value. */
        int PosDeltaZ { 0 }; /* Mouse wheel scroll delta (relative to previus readed) value. */
    };

    struct keyboard_data
    {
        /* Keys state array getter function.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[VK_CONTROL] or GetKeys()['A'] */
        bool Keys[256] { 0 };

        /* Keys previously readed state array getter function.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[VK_CONTROL] or GetKeys()['A'] */
        bool KeysOld[256] { 0 };

        /* Keys click state array getter function.
           Click state is equal to true only once when user pressed down key.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[VK_CONTROL] or GetKeys()['A'] */
        bool KeysClick[256] { 0 };
    };

    class input_system
    {
    protected: /* Available devices data. */
        mouse_data Mouse {};
        keyboard_data Keyboard {};

    public: /* Available devices getters. */
        const mouse_data &GetMouse() const { return Mouse; }
        const keyboard_data &GetKeyboard() const { return Keyboard; }

    public: /* Class methods. */
        /**
         * Read all available human devies function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Response() {}

        /**
         * Create input system depends on platform function.
         * 
         * \param None.
         * \Return Created input system pointer.
         */
        static unique<input_system> Create();
    };
}
