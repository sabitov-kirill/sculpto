/*****************************************************************//**
 * \file   application.h
 * \brief  
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "sclpch.h"
#include "platform/windows/window.h"

namespace scl
{
    class SCL_API application: public window
    {
    public:
        /**
         * Default applicatino construcotr.
         * Initialises all depended systems.
         * 
         * \param Name - Application name. Will be shown in window title.
         */
        application(const std::string &Name);

        /* Application destructor. Deinitialises all depended systems. */
        virtual ~application() = default;

        /**
         * Run application loop and show window function.
         * 
         * \param None.
         * \return None.
         */
        void Run();
    };

    extern application *CreateApplication();
}
