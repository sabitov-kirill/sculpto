/*****************************************************************//**
 * \file   window.cpp
 * \brief  Window class realisation module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "window.h"

 /**
  * Default window construcotor.
  * ! Initialise window, but don't shows it and don't start message loop.
  * If initialisation successful 'IsStartupSuccess' would be set to true
  * and then window message loop could be started.
  *
  * \param Width - Width of the creating window in pixels.
  * \param Height - Height of creating window in pixels.
  * \param Title - Title of the creating window.
  * \return Initialisation success flag.
  */
scl::window::window(int Width, int Height, const std::string &Title)
{
    // Create window class
    WNDCLASS window_class {};

    const std::string window_class_name = Title + "Window Class Name.";
    window_class.lpszClassName = window_class_name.c_str();
    window_class.lpszMenuName = nullptr;
    window_class.style = CS_VREDRAW | CS_HREDRAW;

    window_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
    window_class.hCursor = LoadCursor(InstanceHandle, IDC_CROSS);
    window_class.hIcon = LoadIcon(InstanceHandle, IDI_APPLICATION);
    window_class.hInstance = InstanceHandle;

    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = sizeof(window *);
    window_class.lpfnWndProc = WinProc;

    // Register window class, check if succsess
    SCL_ASSERT(RegisterClass(&window_class));

    // Create window
    Handle = CreateWindow(
        window_class_name.c_str(), Title.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, InstanceHandle, reinterpret_cast<void *>(this)
    );
}

/**
 * Starting window message loop function.
 * Probably messages pumping will be added.
 *
 * \returns None.
 */
void scl::window::Run()
{
    ShowWindow(Handle, SW_SHOWNORMAL);

    // Run message loop.
    MSG msg;
    while (true)
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else Timer();
}

LRESULT WIN32 scl::window::WinProc(window_handle hWnd, u32 Msg,
                                   WPARAM wParam, LPARAM lParam)
{
    // Set window creation parametrs
    if (Msg == WM_CREATE)
        SetWindowLongPtr(
            hWnd, 0,
            (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams
        );

    // Get win class pointer, connected to Win32 window window_handle
    window *win = reinterpret_cast<window *>(GetWindowLongPtr(hWnd, 0));

    // Messages cracking
    switch (Msg)
    {
        HANDLE_MSG(hWnd, WM_CREATE, win->OnCreate);
        HANDLE_MSG(hWnd, WM_SIZE, win->OnSize);
        HANDLE_MSG(hWnd, WM_ERASEBKGND, win->OnEraseBgr);
        HANDLE_MSG(hWnd, WM_PAINT, win->OnPaint);
        HANDLE_MSG(hWnd, WM_TIMER, win->OnTimer);
        HANDLE_MSG(hWnd, WM_DESTROY, win->OnDestroy);
        HANDLE_MSG(hWnd, WM_CLOSE, win->OnClose);

        HANDLE_MSG(hWnd, WM_MOUSEWHEEL, win->OnMWheel);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_RBUTTONDOWN, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_MBUTTONDOWN, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_LBUTTONDBLCLK, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_RBUTTONDBLCLK, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_MBUTTONDBLCLK, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, win->OnMButUp);
        HANDLE_MSG(hWnd, WM_RBUTTONUP, win->OnMButUp);
        HANDLE_MSG(hWnd, WM_MBUTTONUP, win->OnMButUp);
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
