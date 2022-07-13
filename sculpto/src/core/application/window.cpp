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

scl::window::window(int Width, int Height, const std::string &Title) :
    Data(Width, Height, Title)
{
}

scl::unique<scl::window> scl::window::Create(int Width, int Height, const std::string &Title)
{
#ifdef SCL_PLATFORM_WINDOWS
    return CreateUnique<windows_window>(Width, Height, Title);
#else /* !SCL_PLATFORM_WINDOWS */
    SCL_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}
