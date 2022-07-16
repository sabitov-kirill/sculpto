/*****************************************************************//**
 * \file   window.cpp
 * \brief  Window class realisation module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include "sclpch.h"

#include <backends/imgui_impl_win32.h>

#include "windows_window.h"

void scl::windows_window::Create()
{
    // Create window class
    WNDCLASS window_class {};

    const std::string window_class_name = Data.Title + "Window Class Name.";
    window_class.lpszClassName = window_class_name.c_str();
    window_class.lpszMenuName = nullptr;
    window_class.style = CS_VREDRAW | CS_HREDRAW;

    window_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
    window_class.hCursor = LoadCursor(InstanceHandle, IDC_CROSS);
    window_class.hIcon = LoadIcon(InstanceHandle, IDI_APPLICATION);
    window_class.hInstance = InstanceHandle;

    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = sizeof(windows_window *);
    window_class.lpfnWndProc = WinProc;

    // Register window class, check if succsess
    SCL_CORE_ASSERT(RegisterClass(&window_class), "Window class creation failed.");

    // Create window
    Handle = CreateWindow(
        window_class_name.c_str(), Data.Title.c_str(),
        WS_OVERLAPPEDWINDOW | WM_SHOWWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, InstanceHandle, reinterpret_cast<void *>(this)
    );
    ShowWindow(Handle, 1);
    SCL_CORE_SUCCES("Windows window created.");
}

scl::windows_window::windows_window(int Width, int Height, const std::string &Title) :
    window(Width, Height, Title)
{
    this->Create();
}

void scl::windows_window::Update()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void scl::windows_window::FlipFullscreen()
{
}

void scl::windows_window::ChangeTitle(const std::string &NewTitle)
{
    SetWindowText(Handle, NewTitle.c_str());
}

void scl::windows_window::ShutDown()
{
    PostMessage(Handle, WM_CLOSE, 0, 0);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WIN32 scl::windows_window::WinProc(window_handle hWnd, u32 Msg,
                                           WPARAM wParam, LPARAM lParam)
{
    // Set window creation parametrs
    if (Msg == WM_CREATE)
        SetWindowLongPtr(
            hWnd, 0,
            (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams
        );

    if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
        return true;

    // Get win class pointer, connected to Win32 window window_handle
    windows_window *win = reinterpret_cast<windows_window *>(GetWindowLongPtr(hWnd, 0));

    // Messages cracking
    switch (Msg)
    {
        HANDLE_MSG(hWnd, WM_CREATE, win->OnCreate);
        HANDLE_MSG(hWnd, WM_SIZE, win->OnSize);
        HANDLE_MSG(hWnd, WM_DESTROY, win->OnDestroy);
        HANDLE_MSG(hWnd, WM_CLOSE, win->OnClose);
        HANDLE_MSG(hWnd, WM_TIMER, win->OnTimer);

        HANDLE_MSG(hWnd, WM_ERASEBKGND, win->OnEraseBackground);
        HANDLE_MSG(hWnd, WM_PAINT, win->OnPaint);
        HANDLE_MSG(hWnd, WM_ACTIVATE, win->OnActivate);

        HANDLE_MSG(hWnd, WM_MOUSEMOVE, win->OnMMove);
        HANDLE_MSG(hWnd, WM_MOUSEWHEEL, win->OnMWheel);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_LBUTTONDBLCLK, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, win->OnMButUp);
        HANDLE_MSG(hWnd, WM_RBUTTONDOWN, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_RBUTTONDBLCLK, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_RBUTTONUP, win->OnMButUp);
        HANDLE_MSG(hWnd, WM_MBUTTONDOWN, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_MBUTTONDBLCLK, win->OnMButDown);
        HANDLE_MSG(hWnd, WM_MBUTTONUP, win->OnMButUp);

        HANDLE_MSG(hWnd, WM_KEYDOWN, win->OnKey);
        HANDLE_MSG(hWnd, WM_KEYUP, win->OnKey);
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
