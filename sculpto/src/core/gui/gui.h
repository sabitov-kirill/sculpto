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
    class gui
    {
    public: /* Application graphics user interface layer data. */
        static bool IsDockspace;

    public:
        /* Default layer constructor. */
        gui() = default;

        /* Default layer destcuctor. */
        ~gui() = default;

        /**
         * Layer initialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        static void Init();

        /**
         * Layer deinitialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        static void Close();

        /**
         * Begining GUI rendering pipeline function.
         * 
         * \param None.
         * \return None.
         */
        static void BeginUpdate();

        /**
         * Submit rendered GUI function.
         * 
         * \param None.
         * \return None.
         */
        static void SubmitUpdate();

    private:
        /**
         * Draw docking space to window function.
         *
         * \param None.
         * \return None.
         */
        static void DrawDockspace();

        /**
         * Setup Dear ImGui color scheme function.
         *
         * \param None.
         * \return None.
         */
        static void SetUpTheme();
    };
}