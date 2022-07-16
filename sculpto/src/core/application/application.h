/*****************************************************************//**
 * \file   application.h
 * \brief  
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "timer.h"
#include "window.h"
#include "layers_stack.h"
#include "../events/events.h"
#include "../gui/gui.h"

/**
 * Main programm function.
 *
 * \param argc - application run arguments count.
 * \param argv - application run arguments array.
 * \return system return code.
 */
int main(int argc, char *argv[]);

namespace scl
{
    class application abstract
    {
        friend int ::main(int argc, char *argv[]);

    private: /* Application data. */
        static application *Instance;            /* Global application instance. */

        bool           IsRunning { true };  /* Application main loop running flag. */
        unique<window> Window {};           /* Main app window. */

    public:
        bool GuiEnabled { false };          /* Application graphic user interface enabled flag. If false do not rendering gui. */

    public: /* Applicatino getters/setters functions. */
        /* Get applciation instance function. */
        static application &Get() { return *Instance; }
        /* Window getter function. */
        const window &GetWindow() const { return *Window; }

    private: /* Private methods. */
        /**
         * Per main application loop actions function.
         * 
         * \param None.
         * \return None.
         */
        void LoopIterationActions();

    public: /* Client application virtual collbacks. */
        /**
         * Application initialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual void OnInit() {}

        /**
         * Application deinitialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual void OnClose() {}

        /**
         * Application update virtual callback function.
         * 
         * \param DeltaTime - update inter frame delta time.
         * \retunr None.
         */
        virtual void OnUpdate(float DeltaTime) {}

        /**
         * Application GUI update virtual callback function.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnGuiUpdate() {}

    public: /* Application methods. */
        /**
         * Default applicatino construcotr.
         * Initialises all depended systems.
         *
         * \param Name - Application name. Will be shown in window title.
         */
        application(const std::string &Name);

        /* Default application destructor. */
        virtual ~application();

        /**
         * Run application loop and show window function.
         * 
         * \param None.
         * \return None.
         */
        void Run();

        /**
         * Application shut down function.
         * 
         * \param None.
         * \return None.
         */
        void ShutDown();
    };

    extern application *CreateApplication();
}
