/*!****************************************************************//*!*
 * \file   input.h
 * \brief  Abstract, platform independent input system definition mpdule.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "input_keycodes.h"

namespace scl
{
    struct mouse_data
    {
        int PosX { 0 };      /*! Mouse x absolute screen position. */
        int PosY { 0 };      /*! Mouse y absolute screen position. */
        int PosZ { 0 };      /*! Mouse wheel absolute scrool value. */
        int PosDeltaX { 0 }; /*! Mouse x screen position delta (relative to previus readed) value. */
        int PosDeltaY { 0 }; /*! Mouse y screen position delta (relative to previus readed) value. */
        int PosDeltaZ { 0 }; /*! Mouse wheel scroll delta (relative to previus readed) value. */
    };

    struct keyboard_data
    {
        /*! Keys state array getter function.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[keycodes::...] or GetKeys()['A'] */
        u8 Keys[256] { 0 };

        /*! Keys previously readed state array getter function.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[keycodes::...] or GetKeys()['A'] */
        u8 KeysOld[256] { 0 };

        /*! Keys click state array getter function.
           Click state is equal to true only once when user pressed down key.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[keycodes::...] or GetKeys()['A'] */
        u8 KeysClick[256] { 0 };
    };

    class input_system
    {
    private: /*! Input system data. */
        static shared<input_system> Instance;

    protected:
        mouse_data Mouse;
        keyboard_data Keyboard;

    public: /*! Input system data getter/setter functions. */
        /*! Mouse x absolute screen position getter function. */
        static int GetMousePosX() { return Instance->Mouse.PosX; }
        /*! Mouse y absolute screen position getter function. */
        static int GetMousePosY() { return Instance->Mouse.PosY; }
        /*! Mouse wheel absolute scrool value getter function. */
        static int GetMousePosZ() { return Instance->Mouse.PosZ; }
        /*! Mouse x screen position delta (relative to previus readed) value getter function. */
        static int GetMousePosDeltaX() { return Instance->Mouse.PosDeltaX; }
        /*! Mouse y screen position delta (relative to previus readed) value getter function. */
        static int GetMousePosDeltaY() { return Instance->Mouse.PosDeltaY; }
        /*! Mouse wheel scroll delta (relative to previus readed) value getter function. */
        static int GetMousePosDeltaZ() { return Instance->Mouse.PosDeltaZ; }

        /*! Keys state array getter function.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[keycode::SPACE] or GetKeys()['A'] */
        static bool GetKey(keycode Index) { return Instance->Keyboard.Keys[(int)Index]; };

        /*! Keys previously readed state array getter function.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[keycode::SPACE] or GetKeys()['A'] */
        static bool GetKeyOld(keycode Index) { return Instance->Keyboard.KeysOld[(int)Index]; };

        /*! Keys click state array getter function.
           Click state is equal to true only once when user pressed down key.
           Key state could be accessed from array by its VK_*** key code or letter char.
           Example: GetKeys()[keycode::SPACE] or GetKeys()['A'] */
        static bool GetKeyClick(keycode Index) { return Instance->Keyboard.KeysClick[(int)Index]; };

        /*!*
         * Read all available human devies function.
         *
         * \param None.
         * \return None.
         */
        static void Response() { Instance->SelfResponse(); }

    public: /*! Class methods. */
        /*!*
         * Input system instance reference getter function.
         * 
         * \param None.
         * \return input system instance reference.
         */
        static shared<input_system> &Get() { return Instance; }

        /*!*
         * Create input system depends on platform function.
         * 
         * \param None.
         * \Return Created input system pointer.
         */
        static unique<input_system> Create();

    protected:
        /*!*
         * Read all available human devies function.
         *
         * \param None.
         * \return None.
         */
        virtual void SelfResponse() {}
    };
}
