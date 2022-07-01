/*****************************************************************//**
 * \file   window.cpp
 * \brief  Abstract, platform independent class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#include "sclpch.h"

#ifdef SCL_PLATFORM_WINDOWS
#   include "platform/windows/windows_window.h"
#endif

scl::window::window(int Width, int Height, const std::string &Title, bool VSync) :
    Data(Width, Height, Title, false)
{
    Input = input_system::Create();
}

scl::unique<scl::window> scl::window::Create(int Width, int Height, const std::string &Title, bool VSync)
{
#ifdef SCL_PLATFORM_WINDOWS
    return CreateUnique<windows_window>(Width, Height, Title, VSync);
#else /* !SCL_PLATFORM_WINDOWS */
    SCL_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}

scl::unique<scl::window> scl::window::Create(int Width, int Height, const std::string &Title, bool VSync, const events_handler &EventHandler)
{
#ifdef SCL_PLATFORM_WINDOWS
    return CreateUnique<windows_window>(Width, Height, Title, VSync, EventHandler);
#else /* !SCL_PLATFORM_WINDOWS */
    SCL_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}
