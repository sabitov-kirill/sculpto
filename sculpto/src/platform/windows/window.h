/*****************************************************************//**
 * \file   window.h
 * \brief  Window window_handle definition module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "input.h"

namespace scl
{
    class SCL_API window
    {
        /* Window consfig data structure. */
        struct SCL_API data
        {
        public:
            /* Window size properties */
            int  Width, Height;
            /* Virtual synchrosisation enable flag. */
            bool VSync;

            /* Constructors */
            data() = default;
            data(const data &Other) = default;
            data(int Width, int Height, bool VSync) :
                Width(Width), Height(Height), VSync(VSync) {}
        };

    protected: /* Window data. */
        bool IsInitialised { false };
        bool IsActive { false };
        bool IsFullscreen { false };
        data Data {};
        input_system Input { Handle, MouseWheel };

    private: /* Win32 window data. */
        window_handle   Handle {};
        instance_handle InstanceHandle {};
        timer_handle    InitialisationTimerId { 1 };
        timer_handle    ResponseTimerId { 30 };

        int MouseWheel { 0 };

    private: /* Main window procedure. */
        static LRESULT WIN32 WinProc(window_handle hWnd, u32 Msg,
                                     WPARAM wParam, LPARAM lParam);

    private: /* Window size data. */
        rect FullscreenRect {};

    private: /* Low level window message crackers.
              * Executed from main window main procedure if corresponding message dispatched.
              */
        bool OnCreate(window_handle WindowHandle, CREATESTRUCT *CS);
        void OnSize(window_handle WindowHandle, u32 State, int W, int H);
        void OnTimer(window_handle WindowHandle, int Id);
        void OnMWheel(window_handle WindowHandle, int X, int Y, int Z, u32 Keys);
        void OnMButDown(window_handle WindowHandle, bool IsDblClick, int X, int Y, u32 Keys);
        void OnMButUp(window_handle WindowHandle, int X, int Y, u32 Keys);
        bool OnEraseBgr(window_handle WindowHandle, dc_handle drawing_context);
        void OnPaint(window_handle WindowHandle);
        void OnActivate(window_handle WindowHandle, u32 Reason, window_handle hWndActDeact, bool IsMinimized);
        bool OnClose(window_handle WindowHandle);
        void OnDestroy(window_handle WindowHandle);

    protected: /* Windows window overridable specific callbacks. */
        /**
         * Window Initialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual VOID Init() {}

        /**
         * Window Closing virtual callback function..
         *
         * \param None.
         * \return None.
         */
        virtual VOID Close() {}

        /**
         * Window Resize virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual VOID Resize() {}

        /**
         * Window Timer tick virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual VOID Timer() {}

        /**
         * Window Activation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual VOID Activate() {}

        /**
         * Window background Erase virtual callback function.
         * 
         * \param DrawContext - drawing context window_handle.
         * \return None.
         */
        virtual VOID Erase(dc_handle DrawContext) {}

         /**
          * Window paint ability virtual callback function.
          * 
          * \param DrawContext - drawing context window_handle.
         * \return None.
          */
        virtual VOID Paint(dc_handle DrawContext) {}

    public: /* Class methods. */
        /***
         * Copying and moving constructors,
         * lvalue and rvalue copy assigment and move assigment operators
         * removal due to window concept (window loop execution at runtime).
         ***/

        window(const window &) = delete;
        window(const window &&) = delete;
        window &operator=(const window &) = delete;
        window &operator=(const window &&) = delete;
        window operator=(const window &) const = delete;
        window operator=(const window &&) const = delete;

        /**
         * Windows OS specific window construcotor.
         * Initialise window, but don't shows it and don't start message loop.
         * If initialisation successful 'IsStartupSuccess' would be set to true
         * and then window message loop could be started.
         *
         * \param Width - Width of the creating window in pixels.
         * \param Height - Height of creating window in pixels.
         * \param Name - Title of the creating window.
         */
        window(int Width, int Height, const std::string &Title);

        /**
         * Starting window message loop function.
         * Probably messages pumping will be added.
         *
         * \param None.
         * \returns None.
         */
        void Run();

        /**
         * Woggle window display mode (fullscreen/windowed) function.
         *
         * \param None.
         * \return None.
         */
        void FlipFullscreen() {}
        
        /**
         * Change window title to specified.
         * 
         * \param NewTitle - new window title.
         * \return None.
         */
        void ChangeTitle(const std::string &NewTitle) {}

    public: /* Class getters/setters */
        /* Window handle getter function. */
        window_handle GetWindowHandle() { return Handle; }
        /* Window data getting function. */
        const data &GetWindowData() const { return Data; }
        /* Window size setter function. */
        void SetSize(int NewWidth, int NewHeight) { Data.Width = NewWidth; Data.Height = NewHeight; }
        /* Window VSync falg setter function. */
        void SetVsync(bool NewVSync) { Data.VSync = NewVSync; }
    };
}