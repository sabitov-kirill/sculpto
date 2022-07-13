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
#include "../gui/gui_layer.h"

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
        unique<window> Window;              /* Main app window. */
        layers_stack   Layers {};           /* Application layers stack. */

    public:
        bool      GuiEnabled { true }; /* Application graphic user interface enabled flag. If false do not rendering gui. */
        gui_layer *GUILayer {};         /* Application graphic user interface layer pointer. */

    public: /* Applicatino getters/setters functions. */
        /* Get applciation instance function. */
        static application &Get() { return *Instance; }
        /* Window getter function. */
        const window &GetWindow() const { return *Window; }

    private: /* Private methods. */
        /**
         * Application on window resize callback function.
         * 
         * \param Event - window resize event.
         * \return whether event was can be handled by next event handlers or not.
         */
        bool OnWindowResize(window_resize_event &Event);

        /**
         * Application on window close callback function.
         * 
         * \param Event - window close event.
         * \return whether event was can be handled by next event handlers or not.
         */
        bool OnWindowClose(window_close_event &Event);

        /**
         * Per main application loop actions function.
         * 
         * \param None.
         * \return None.
         */
        void LoopIterationActions();

    public: /* Client application virtual collbacks. */
        /**
         * Layer initialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual void OnInit() {}

        /**
         * Layer deinitialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual void OnClose() {}

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
         * Push common layer to stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void Push(layer *Layer) { Layers.Push(Layer); }

        /**
         * Push overlay layer (e.g. GUI) to stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void PushOverlay(layer *Layer) { Layers.PushOverlay(Layer); }

        /**
         * Pop common layer from stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void Pop(layer *Layer) { Layers.Pop(Layer); }

        /**
         * Pop overlay layer (e.g. GUI) from stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void PopOverlay(layer *Layer) { Layers.PopOverlay(Layer); }

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
