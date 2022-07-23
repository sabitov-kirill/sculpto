/*!****************************************************************//*!*
 * \file   windows_input.cpp
 * \brief  Input window_handle realisation module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>

#include "windows_input.h"
#include "core/application/application.h"

void scl::windows_input_system::MouseInit()
{
    // Obtain position
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(*WindowHandle, &pt);

    // Absolute values
    Mouse.PosY = pt.x;
    Mouse.PosX = pt.y;
}

void scl::windows_input_system::MouseRead()
{
    // Obtain position
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(*WindowHandle, &pt);

    // Delta (relative to previus readed) values
    Mouse.PosDeltaX = pt.x - Mouse.PosX;
    Mouse.PosDeltaY = pt.y - Mouse.PosY;

    // Absolute values
    Mouse.PosX = pt.x;
    Mouse.PosY = pt.y;

    // Wheel (z) value
    if (*Wheel != 0)
    {
        Mouse.PosDeltaZ = *Wheel * (!ImGui::GetIO().WantCaptureMouse);
        Mouse.PosZ += *Wheel;
        *Wheel = 0;
    }
    else Mouse.PosDeltaZ = 0;
}

void scl::windows_input_system::KeyboardInit()
{
    if (GetKeyboardState((PBYTE)Keyboard.Keys))
        memcpy(Keyboard.KeysOld, Keyboard.Keys, 256);
}

void scl::windows_input_system::KeyboardRead()
{
    memcpy(Keyboard.KeysOld, Keyboard.Keys, 256);
    bool _ = GetKeyboardState((PBYTE)Keyboard.Keys);

    ImGuiIO io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        Keyboard.Keys[(int)keycode::LBUTTON] =
        Keyboard.Keys[(int)keycode::RBUTTON] =
        Keyboard.Keys[(int)keycode::MBUTTON] = 0;

    for (INT i = 0; i < 256; i++)
    {
        Keyboard.Keys[i] >>= 7;
        Keyboard.KeysClick[i] = Keyboard.Keys[i] && !Keyboard.KeysOld[i];
    }
    if (io.WantCaptureKeyboard)
    {
        char lbut = Keyboard.Keys[(int)keycode::LBUTTON];
        char rbut = Keyboard.Keys[(int)keycode::RBUTTON];
        char mbut = Keyboard.Keys[(int)keycode::MBUTTON];
    
        memset(Keyboard.Keys, 0, 256), memset(Keyboard.KeysClick, 0, 256);
        Keyboard.Keys[(int)keycode::LBUTTON] = lbut;
        Keyboard.Keys[(int)keycode::RBUTTON] = rbut;
        Keyboard.Keys[(int)keycode::MBUTTON] = mbut;
    }
}

void scl::windows_input_system::Init(window_handle *WindowHandle, int *MouseWheel)
{
    this->WindowHandle = WindowHandle;
    this->Wheel = MouseWheel;
}

void scl::windows_input_system::SelfResponse()
{
    if (WindowHandle == nullptr || Wheel == nullptr) return;

    MouseRead();
    KeyboardRead();
}
