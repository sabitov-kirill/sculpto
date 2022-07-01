/*****************************************************************//**
 * \file   input.h
 * \brief  Abstract, platform independent input system definition mpdule.
 *
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "platform/windows/windows_input.h"

scl::unique<scl::input_system> scl::input_system::Create()
{
#ifdef SCL_PLATFORM_WINDOWS
    return CreateUnique<windows_input_system>();
#else /* !SCL_PLATFORM_WINDOWS */
    SCL_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}
