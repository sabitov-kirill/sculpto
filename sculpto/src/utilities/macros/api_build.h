/*****************************************************************//**
 * \file   api_build.h
 * \brief  Macroses for building and including Sculpto library
 *         to third party apps definition module.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#ifdef SCL_PLATFORM_WINDOWS
#   ifdef SCL_BUILD_DLL
#       define SCL_API __declspec(dllexport)
#   else
#       define SCL_API __declspec(dllimport)
#   endif /* !SCL_BUILD_DLL */
#else
#   error Currently only supported OS us Windows!
#endif /* !SCL_PLATFORM_WINDOWS */
