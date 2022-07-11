/*****************************************************************//**
 * \file   application_config_window.h
 * \brief  Application configuration GUI window class definition module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Application configuration GUI window class. */
    class application_config_window
    {
    private:
        vec4 ClearColor {};
        bool IsWireframe {};

    public:
        /* Application configuration GUI window default constructor. */
        application_config_window();

        /**
         * Application configuration GUI window draw function.
         * 
         * \param None.
         * \return None.
         */
        void Draw();
    };
}
