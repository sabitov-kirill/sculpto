/*****************************************************************//**
 * \file   windows_msg_crackers.cpp
 * \brief  Window class message crackers realisation module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "windows_window.h"

bool scl::windows_window::OnCreate(window_handle WindowHandle, CREATESTRUCT *CS)
{
    // Set window window_handle to newly created one
    Handle = WindowHandle;
    reinterpret_cast<windows_input_system *>(input_system::Get().get())->Init(&Handle, &MouseWheel);

    SetTimer(Handle, InitialisationTimer, 0, nullptr);
    return 1;
}

void scl::windows_window::OnSize(window_handle WindowHandle, u32 State, int W, int H)
{
    // Set current window sizing
    Data.Width = W;
    Data.Height = H;

    if (!IsInitialised) return;

    // Call resize callback
    window_resize_event e { W, H };
    event_dispatcher::Invoke(e);
}

bool scl::windows_window::OnClose(window_handle WindowHandle)
{
    DestroyWindow(Handle);
    return 0;
}

void scl::windows_window::OnDestroy(window_handle WindowHandle)
{
    if (IsInitialised)
    {
        window_close_event e {};
        event_dispatcher::Invoke(e);
    }

    PostQuitMessage(30);
}

void scl::windows_window::OnTimer(window_handle WindowHandle, UINT id)
{
    if (IsInitialised) return;

    IsInitialised = true;
    KillTimer(Handle, InitialisationTimer);

    // Call resize callback
    window_resize_event e { Data.Width, Data.Height };
    event_dispatcher::Invoke(e);
}

bool scl::windows_window::OnEraseBackground(window_handle WindowHandle, HDC hDC)
{
    return true;
}

void scl::windows_window::OnPaint(window_handle WindowHandle)
{
    PAINTSTRUCT ps;
    HDC hDC;

    hDC = BeginPaint(WindowHandle, &ps);
    EndPaint(WindowHandle, &ps);
}

void scl::windows_window::OnActivate(window_handle WindowHandle, u32 Reason, HWND hWndActDeact, bool IsMinimized)
{
}

void scl::windows_window::OnMMove(HWND hwnd, int x, int y, u32 keyFlags)
{
    if (!IsInitialised) return;

    mouse_move_event e {
        x, y,
        (bool)(keyFlags & MK_RBUTTON), (bool)(keyFlags & MK_LBUTTON), (bool)(keyFlags & MK_MBUTTON),
        (bool)(keyFlags & MK_SHIFT), (bool)(keyFlags & MK_CONTROL)
    };
    event_dispatcher::Invoke(e);
}

void scl::windows_window::OnMWheel(window_handle WindowHandle, int X, int Y, int Z, u32 Keys)
{
    if (!IsInitialised) return;

    this->MouseWheel += Z;
    input_system::Response();

    mouse_wheel_event e { Z };
    event_dispatcher::Invoke(e);
}

void scl::windows_window::OnMButDown(window_handle WindowHandle, bool IsDblClick, int X, int Y, u32 Keys)
{
    if (!IsInitialised) return;

    mouse_button_event e { true, X, Y, (mouse_button)Keys };
    event_dispatcher::Invoke(e);
}

void scl::windows_window::OnMButUp(window_handle WindowHandle, int X, int Y, u32 Keys)
{
    if (!IsInitialised) return;

    mouse_button_event e { false, X, Y, (mouse_button)Keys };
    event_dispatcher::Invoke(e);
}

void scl::windows_window::OnKey(HWND hwnd, u32 vk, bool fDown, int cRepeat, u32 flags)
{
    if (!IsInitialised) return;

    keyboard_event e { (bool)fDown, (bool)(flags & (1 << 14)), (keycode)vk};
    event_dispatcher::Invoke(e);
}
