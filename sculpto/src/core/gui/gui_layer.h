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
         * Layer event handling virtual callback function.
         *
         * \param Event - event to handle.
         * \return None.
         */
        void OnEvent(event &Event) override;

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

        /**
         * Setup Dear ImGui color scheme function.
         * 
         * \param None.
         * \return None.
         */
        void SetUpTheme();
    };
}