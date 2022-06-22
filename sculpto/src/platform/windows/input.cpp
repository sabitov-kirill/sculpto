/*****************************************************************//**
 * \file   windows_input.cpp
 * \brief  Input window_handle realisation module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include "sclpch.h"
 #include "input.h"

/**
 * Mouse default constructor.
 *
 * \param WindowHandle - handle of window using interacting with.
 * \param Wheel - reference to value representing mouse wheel state.
 *                (need since it can't be obtaint by WinMM lib.)
 */
scl::mouse::mouse(window_handle &WindowHandle, int &Wheel) :
    WindowHandle(WindowHandle), Wheel(Wheel)
{
    // Obtain position
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(WindowHandle, &pt);

    // Absolute values
    PosY = pt.x;
    PosX = pt.y;
}

/**
 * Read human device interface data.
 *
 * \param None.
 * \returns None.
 */
void scl::mouse::Read()
{
    // Obtain position
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(WindowHandle, &pt);

    // Delta (relative to previus readed) values
    PosDeltaX = pt.x - PosX;
    PosDeltaY = pt.y - PosY;

    // Absolute values
    PosY = pt.x;
    PosX = pt.y;

    // Wheel (z) value
    PosDeltaZ = Wheel;
    PosZ += PosDeltaZ;
    Wheel = 0;
}

/* keyboard default constructor. */
scl::keyboard::keyboard()
{

    if (GetKeyboardState((PBYTE)Keys))
        memcpy(KeysOld, Keys, 256);
}

/**
 * Read mouse data concidering window it was created with.
 *
 * \param None.
 * \returns None.
 */
void scl::keyboard::Read()
{
    if (GetKeyboardState((PBYTE)Keys))
    {
        for (INT i = 0; i < 256; i++)
            KeysClick[i] = Keys[i] && !KeysOld[i];
        memcpy(KeysOld, Keys, 256);
    }
}


/**
 * Windows specific input system default constructor.
 *
 * \param WindowHandle - handle of window using interacting with.
 * \param MouseWheel - reference to value representing mouse wheel state
 *                     (need since it can't be obtaint by WinMM lib.)
 */
scl::input_system::input_system(window_handle WindowHandle, int &MouseWheel) :
    Mouse(WindowHandle, MouseWheel) {}
