/*****************************************************************//**
 * \file   windows_input.h
 * \brief  Input window_handle definition module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "core/application/input.h"

namespace scl
{
    /* Windows specific input system class. */
    class windows_input_system: public input_system
    {
    private:
        window_handle *WindowHandle {};
        int *Wheel {};

        void MouseInit();
        void MouseRead();

        void KeyboardInit();
        void KeyboardRead();

    public:
        /**
         * Windows specific input system initialisation function.
         * 
         * \param WindowHandle - handle of window using interacting with.
         * \param MouseWheel - reference to value representing mouse wheel state
         *                     (need since it can't be obtaint by WinMM lib.)
         */
        void Init(window_handle *WindowHandle, int *MouseWheel);

    protected:
        /**
         * Read all available human devies function.
         *
         * \param None.
         * \return None.
         */
        void SelfResponse() override;
    };
}
