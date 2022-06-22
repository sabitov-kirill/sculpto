/*****************************************************************//**
 * \file   windows_input.h
 * \brief  Input window_handle definition module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "sclpch.h"

namespace scl
{
    /**
     * \brief Pure virtual human interface device class.
     *        Any HID could be readed and store neccessery information.
     *        Realisation and data model depends by platform.
     */
    class human_interface_device abstract
    {
    public:
        /**
         * Read human device interface data.
         *
         * \param None.
         * \returns None.
         */
        virtual void Read() = 0;
    };

    /* Mouse human interface devise class. */
    class SCL_API mouse : public human_interface_device
    {
    private: /* Mouse data. */
        window_handle &WindowHandle;
        int &Wheel;
        int
            PosX { 0 },
            PosY { 0 },
            PosZ { 0 },
            PosDeltaX { 0 },
            PosDeltaY { 0 },
            PosDeltaZ { 0 };

    public:
        /**
         * Mouse default constructor.
         *
         * \param WindowHandle - handle of window using interacting with.
         * \param Wheel - reference to value representing mouse wheel state
         *                (need since it can't be obtaint by WinMM lib.)
         */
        mouse(window_handle &WindowHandle, int &Wheel);

        /**
          * Read mouse data concidering window it was created with.
          *
          * \param None.
          * \returns None.
          */
        void Read() override final;

    public: /* Data getters. */
        /* Mouse x absolute screen position getter function. */
        int GetPosX() { return PosX; }
        /* Mouse y absolute screen position getter function. */
        int GetPosY() { return PosY; }
        /* Mouse wheel absolute scrool value getter function. */
        int GetPosZ() { return PosZ; }
        /* Mouse x screen position delta (relative to previus readed) value getter function. */
        int GetPosDeltaX() { return PosDeltaX; }
        /* Mouse y screen position delta (relative to previus readed) value getter function. */
        int GetPosDeltaY() { return PosDeltaY; }
        /* Mouse wheel scroll delta (relative to previus readed) value getter function. */
        int GetPosDeltaZ() { return PosDeltaZ; }
    };

    /* Keayboard human interface devise class. */
    class SCL_API keyboard : public human_interface_device
    {
    private:
        bool Keys[256] {0};
        bool KeysOld[256] {0};
        bool KeysClick[256] {0};

    public:
        /* Keyboard default constructor. */
        keyboard();

        /**
         * Read mouse data concidering window it was created with.
         *
         * \param None.
         * \returns None.
         */
        void Read() override final;

    public: /* Data getters. */
        /** 
         * \brief   Keys state array getter function.
         *          Key state could be accessed from array by its VK_*** key code or letter char.
         *          Example: GetKeys()[VK_CONTROL] or GetKeys()['A']
         */
        const bool *GetKeys() const { return Keys; }
        /**
         * \brief   Keys previously readed state array getter function.
         *          Key state could be accessed from array by its VK_*** key code or letter char.
         *          Example: GetKeys()[VK_CONTROL] or GetKeys()['A']
         */
        const bool *GetKeysOld() const { return KeysOld; }
        /**
         * \brief   Keys click state array getter function.
         *          Click state is equal to true only once when user pressed down key.
         *          Key state could be accessed from array by its VK_*** key code or letter char.
         *          Example: GetKeys()[VK_CONTROL] or GetKeys()['A']
         */
        const bool *GetKeysClick() const { return KeysClick; }
    };

    /* Windows specific input system class. */
    class SCL_API input_system
    {
    public: /* Windows available HID's. */
        mouse Mouse;
        keyboard Keyboard;

    public:
        /**
         * Windows specific input system default constructor.
         *
         * \param WindowHandle - handle of window using interacting with.
         * \param MouseWheel - reference to value representing mouse wheel state
         *                (need since it can't be obtaint by WinMM lib.)
         */
        input_system(window_handle WindowHandle, int &MouseWheel);

        /**
         * Read all human devices interfaces data.
         *
         * \param None.
         * \returns None.
         */
        virtual void Read()
        {
            Mouse.Read();
            Keyboard.Read();
        }
    };
}
