/*****************************************************************//**
 * \file   render_adapter.h
 * \brief  Render adapter class definition module.
 *         Currently unused render step. Need to applying post-processing,
 *         sybmiting objects from pool rendering, particle system, etc.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include "render_context.h"

namespace scl
{
    /* Render adapter class. */
    class render_adapter
    {
    private: /* Render adapter data. */
        static render_adapter *Instance;
        unique<render_context> Renderer;
        camera ViewerCamera;

    public: /* Render adapter getter function. */
        /* Render adapter viewer camer getter function. */
        const camera &GetCamera() const { return ViewerCamera; }

    public: /* Render adapter methods. */
        /**
         * Render adapter initialisation function.
         *
         * \param None.
         * \return None.
         */
        void Init();

        /**
         * Render adapter deinitialisation function.
         *
         * \param None.
         * \return None.
         */
        void Close();

        /**
         * Adapter frame start rendering function.
         * 
         * \param None.
         * \return None.
         */
        void RenderFrame();

        /**
         * Adapter frame submit rendered frame function.
         *
         * \param None.
         * \return None.
         */
        void SubmitRenderedFrame();

        /**
         * Render context resize call function.
         * 
         * \param Width, Height - new width and hight of applicatino window.
         * \return None.
         */
        void RenderResize(int Width, int Height);

        /* Adapter instance getter function. */
        static render_adapter &Get()
        {
            if (!Instance) Instance = new render_adapter();
            return *Instance;
        }

        /* Adapter renderer getter function. */
        render_context &GetRenderer() { return *Renderer; }
    };
}
