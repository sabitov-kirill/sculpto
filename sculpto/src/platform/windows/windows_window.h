/*****************************************************************//**
 * \file   window.h
 * \brief  Window window_handle definition module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "core/application/window.h"
#include "windows_input.h"

namespace scl
{
    class windows_window: public window
    {
    private: /* Windows window data. */
        HINSTANCE InstanceHandle {};
        int       MouseWheel {};
        RECT      FullscreenRect {};

        // Initialisation timer need to call resize callback after all application is beeing initialised in application constructor,
        // because resiz windows resize message passed immedeatly after window creation, before windows_window contructor ended.
        // Timer callback called only when application run function is called.
        UINT      InitialisationTimer {};

    private: /* Main window procedure. */
        static LRESULT WIN32 WinProc(window_handle hWnd, u32 Msg,
                                     WPARAM wParam, LPARAM lParam);

    private: /* Low level window message crackers.
              * Executed from main window main procedure if corresponding message dispatched.
              */
        // Window messages
        bool OnCreate(window_handle WindowHandle, CREATESTRUCT *CS);
        void OnSize(window_handle WindowHandle, u32 State, int W, int H);
        bool OnClose(window_handle WindowHandle);
        void OnDestroy(window_handle WindowHandle);
        void OnActivate(window_handle WindowHandle, u32 Reason, HWND hWndActDeact, bool IsMinimized);
        void OnTimer(window_handle WindowHandle, UINT id);

        // Window draw messages
        bool OnEraseBackground(window_handle WindowHandle, HDC hDC);
        void OnPaint(window_handle WindowHandle);

        // Mouse messages
        void OnMMove(HWND hwnd, int x, int y, u32 keyFlags);
        void OnMWheel(window_handle WindowHandle, int X, int Y, int Z, u32 Keys);
        void OnMButDown(window_handle WindowHandle, bool IsDblClick, int X, int Y, u32 Keys);
        void OnMButUp(window_handle WindowHandle, int X, int Y, u32 Keys);

        // Keyboard messages
        void OnKey(HWND hwnd, u32 vk, bool fDown, int cRepeat, u32 flags);

        /**
         * Window creation function.
         * 
         * \param None.
         * \return None.
         */
        void Create();

    public: /* Windows window class methods. */
        /**
         * Windows OS specific window construcotor.
         * Initialise window, but don't shows it and don't start message loop.
         * If initialisation successful 'IsStartupSuccess' would be set to true
         * and then window message loop could be started.
         *
         * \param Width - Width of the creating window in pixels.
         * \param Height - Height of creating window in pixels.
         * \param Name - Title of the creating window.
         * \param VSync - vertical syncrosination enabling flag.
         */
        windows_window(int Width, int Height, const std::string &Title);

        /**
         * Window update function.
         *
         * \param None.
         * \return None.
         */
        void Update() override;

        /**
         * Woggle window display mode (fullscreen/windowed) function.
         *
         * \param None.
         * \return None.
         */
        void FlipFullscreen() override;

        /**
         * Change window title to specified.
         * 
         * \param NewTitle - new window title.
         * \return None.
         */
        void ChangeTitle(const std::string &NewTitle) override;

        /**
         * Window shut down function. Closes window and create window close event.
         *
         * \param None.
         * \return None.
         */
        void ShutDown() override;
    };
}