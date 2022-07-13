/*****************************************************************//**
 * \file   gui_layer.h
 * \brief  Application graphics user interface layer class implementation module.
 *         Handles user GUI events and widgets render.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "../application/layer.h"

namespace scl
{
    /* Application graphics user interface layer class. */
    class gui_layer: public layer
    {
    public: /* Application graphics user interface layer data. */
        static bool IsDockspace;

    public:
        /* Default layer constructor. */
        gui_layer() = default;

        /* Default layer destcuctor. */
        ~gui_layer() override = default;

        /**
         * Layer initialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        void OnInit() override;

        /**
         * Layer deinitialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        void OnClose() override;

        /**
         * Begining GUI rendering pipeline function.
         * 
         * \param None.
         * \return None.
         */
        void RenderGui();

        /**
         * Submit rendered GUI function.
         * 
         * \param None.
         * \return None.
         */
        void SubmitRenderedGui();

    private:
        /**
         * Draw docking space to window function.
         *
         * \param None.
         * \return None.
         */
        void DrawDockspace();

        /**
         * Setup Dear ImGui color scheme function.
         *
         * \param None.
         * \return None.
         */
        void SetUpTheme();
    };
}