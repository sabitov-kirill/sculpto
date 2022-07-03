/*****************************************************************//**
 * \file   window.h
 * \brief  Abstract, platform independent class definition module.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

#include "input.h"
#include "../events/events.h"
#include "utilities/event/event_dispatcher.h"

namespace scl
{
    class window
    {
    public:
        /* Window consfig data structure. */
        struct data
        {
        public:
            /* Window size properties */
            int  Width, Height;
            /* Virtual synchrosisation enable flag. */
            bool VSync;
            /* Window title. */
            std::string Title;

            /* Constructors */
            data() = default;
            data(const data &Other) = default;
            data(int Width, int Height, const std::string &Title, bool VSync) :
                Width(Width), Height(Height), VSync(VSync), Title(Title) {}
        };

        using events_handler = std::function<void(event &)>;

    protected: /* Window data. */
        window_handle        Handle {};
        bool                 IsInitialised { false };
        bool                 IsFullscreen { false };
        data                 Data {};

        events_handler EventHandler = [](event &) {};

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
         * \param VSync - vertical syncrosination enabling flag.
         */
        window(int Width, int Height, const std::string &Title, bool VSync);

        /**
         * Create window depends on platform function.
         *
         * \param Width - Width of the creating window in pixels.
         * \param Height - Height of creating window in pixels.
         * \param Title - Title of the creating window.
         * \param VSync - vertical syncrosination enabling flag.
         * \Return Created window pointer.
         */
        static unique<window> Create(int Width, int Height, const std::string &Title, bool VSync);

        /**
         * Create window depends on platform function.
         *
         * \param Width - Width of the creating window in pixels.
         * \param Height - Height of creating window in pixels.
         * \param Name - Title of the creating window.
         * \param Eventhandler - window events handler function.
         * \Return Created window pointer.
         */
        static unique<window> Create(int Width, int Height, const std::string &Title, bool VSync,
                                     const events_handler &EventHandler);

        /**
         * Window update function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Update() {}

        /**
         * Woggle window display mode (fullscreen/windowed) function.
         *
         * \param None.
         * \return None.
         */
        virtual void FlipFullscreen() {}

        /**
         * Change window title to specified.
         *
         * \param NewTitle - new window title.
         * \return None.
         */
        virtual void ChangeTitle(const std::string &NewTitle) {}

        /**
         * Window shut down function. Closes window and create window close event.
         * 
         * \param None.
         * \return None.
         */
        virtual void ShutDown() = 0;

    public: /* Class getters/setters */
        /* Window data getting function. */
        const data &GetWindowData() const { return Data; }
        /* Window handle getter function. */
        const window_handle &GetHandle() const { return Handle; }
        /* Window initilised flag getter function. */
        bool GetIsInitialised() const { return IsInitialised; }
        /* Window size setter function. */
        void SetSize(int NewWidth, int NewHeight) { Data.Width = NewWidth; Data.Height = NewHeight; }
        /* Window VSync falg setter function. */
        void SetVsync(bool NewVSync) { Data.VSync = NewVSync; }
        /* Window event dispatcher setter function. */
        void SetEventHandler(const events_handler &NewEventHandler) { EventHandler = NewEventHandler; }
    };
}
