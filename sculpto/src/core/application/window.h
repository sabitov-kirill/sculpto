/*!****************************************************************//*!*
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
    /*! Abstarct, platfomr independent window class. */
    class window
    {
    public: /*! Abstract, platform independent window data. */
        /*! Window consfig data structure. */
        struct data
        {
        public:
            /*! Window size properties */
            int  Width, Height;
            /*! Window title. */
            std::string Title;

            /*! Constructors */
            data() = default;
            data(const data &Other) = default;
            data(int Width, int Height, const std::string &Title) :
                Width(Width), Height(Height), Title(Title) {}
        };

        /*! Application main window viewport id. */
        static const int ViewportId = 0;

    protected:
        window_handle Handle {};
        bool          IsInitialised { false };
        bool          IsFullscreen { false };
        data          Data {};

    public: /*! Abstract, platform independent window data getters/setters. */
        /*! Window data getting function. */
        const data &GetWindowData() const { return Data; }
        /*! Window handle getter function. */
        const window_handle &GetHandle() const { return Handle; }
        /*! Window initilised flag getter function. */
        bool GetIsInitialised() const { return IsInitialised; }

    public: /*! Abstratc, platform independent window methods. */
        /*!**
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

        /*!*
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
        window(int Width, int Height, const std::string &Title);

        /*!*
         * Create window depends on platform function.
         *
         * \param Width - Width of the creating window in pixels.
         * \param Height - Height of creating window in pixels.
         * \param Title - Title of the creating window.
         * \param VSync - vertical syncrosination enabling flag.
         * \Return Created window pointer.
         */
        static unique<window> Create(int Width, int Height, const std::string &Title);

        /*!*
         * Window update function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Update() {}

        /*!*
         * Woggle window display mode (fullscreen/windowed) function.
         *
         * \param None.
         * \return None.
         */
        virtual void FlipFullscreen() {}

        /*!*
         * Change window title to specified.
         *
         * \param NewTitle - new window title.
         * \return None.
         */
        virtual void ChangeTitle(const std::string &NewTitle) {}

        /*!*
         * Window shut down function. Closes window and create window close event.
         * 
         * \param None.
         * \return None.
         */
        virtual void ShutDown() = 0;
    };
}
