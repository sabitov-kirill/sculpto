/*****************************************************************//**
 * \file   entry_point.h
 * \brief  Sculpto library entry point definition module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include "sclpch.h"

#ifdef SCL_PLATFORM_WINDOWS

extern scl::application *scl::CreateApplication();

/**
 * Main programm function.
 * 
 * \param hInstance - programm instance handler
 * \param hPrevInstance - previous instance handler
 * \param CmdLine - command line pointer
 * \param ShouldShow - window hide flag
 * \return system return code.
 */
int WINAPI WinMain(_In_ scl::instance_handle hInstance, _In_opt_ scl::instance_handle hPrevInstance,
                    _In_ char *CmdLine, _In_ int ShouldShow)
#endif
{
    auto app = scl::CreateApplication();
    app->Run();
    delete app;
    app = nullptr;
}

#endif /* !SCL_PLATFORM_WINDOWS */

