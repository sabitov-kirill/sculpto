/*****************************************************************//**
 * \file   windows_msg_crackers.cpp
 * \brief  Window class message crackers realisation module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "window.h"

bool scl::window::OnCreate(window_handle WindowHandle, CREATESTRUCT *CS)
{
    // Set window window_handle to newly created one
    Handle = WindowHandle;

    // Creating initialisation timer
    SetTimer(Handle, this->InitialisationTimerId, 0, nullptr);

    return 1;
}

void scl::window::OnSize(window_handle WindowHandle, u32 State, int W, int H)
{
    // Set current window sizing
    Data.Width = W;
    Data.Height = H;

    // Call resize callback
    if (IsInitialised) Resize();
}

void scl::window::OnTimer(window_handle WindowHandle, int Id)
{
    // If initialisation callback alreadty called
    // call common timer callback
    if (IsInitialised) { Timer(); return; }

    // Kill initialisation timer, set common window response timer
    KillTimer(Handle, InitialisationTimerId);
    SetTimer(Handle, ResponseTimerId, 0, nullptr);

    // Call initialisation callbacks
    Init();
    Resize();
    Timer();

    // Change initialisation status flag.
    IsInitialised = true;
}

bool scl::window::OnEraseBgr(window_handle WindowHandle, dc_handle drawing_context)
{
    if (IsInitialised) Erase(drawing_context);
    return true;
}

void scl::window::OnPaint(window_handle WindowHandle)
{
    PAINTSTRUCT ps;
    dc_handle drawing_context;

    drawing_context = BeginPaint(Handle, &ps);
    if (IsInitialised) Paint(drawing_context);
    EndPaint(Handle, &ps);
}

void scl::window::OnActivate(window_handle WindowHandle, u32 Reason, window_handle hWndActDeact, bool IsMinimized)
{
    IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
    if (IsInitialised && IsActive) Activate();
}

bool scl::window::OnClose(window_handle WindowHandle)
{
    DestroyWindow(Handle);
    return 0;
}

void scl::window::OnDestroy(window_handle WindowHandle)
{
    if (IsInitialised) { Close(); KillTimer(Handle, ResponseTimerId); }
    else KillTimer(Handle, InitialisationTimerId);
    PostQuitMessage(30);
}

void scl::window::OnMWheel(window_handle WindowHandle, int X, int Y, int Z, u32 Keys) {}
void scl::window::OnMButDown(window_handle WindowHandle, bool IsDblClick, int X, int Y, u32 Keys) {}
void scl::window::OnMButUp(window_handle WindowHandle, int X, int Y, u32 Keys) {}
