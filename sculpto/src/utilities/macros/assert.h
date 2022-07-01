/*****************************************************************//**
 * \file   assert.h
 * \brief  Assertion macros definition module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

/* Detect SCL platform. */
#include "core/application/platform_detection.h"

/* Debug break macro. */
#ifdef _DEBUG
#   if defined(SCL_PLATFORM_WINDOWS)
#       define SCL_DEBUGBREAK() __debugbreak()
#   elif defined(SCL_PLATFORM_LINUX)
#       include <signal.h>
#       define HZ_DEBUGBREAK() raise(SIGTRAP)
#   else
#       error "Platform doesn't support debugbreak yet!"
#endif
#   define HZ_ENABLE_ASSERTS
#else
#   define HZ_DEBUGBREAK()
#endif

#ifdef SCL_ASSERTION_ENABLED
#   ifdef SCL_PLATFORM_WINDOWS
#       include <intrin.h>
#   endif /* !SCL_PLATFORM_WINDOWS */
    /**
     * Core module assertion function.
     *
     * \param Expr - expression to be calculated.
     *               If 0 - assertion message showed in logger and debug break occures.
     * \param Msg - message to be displayed in case of assertion faield.
     * \return None.
     */
#   define SCL_CORE_ASSERT(Expr, Msg)                                     \
    {                                                                     \
        if (!(Expr))                                                      \
        {                                                                 \
            SCL_CORE_ERROR("Assertion Failed!\n"                          \
                           "Expr: {}\n"                                   \
                           "File: {}\n"                                   \
                           "Line: {}\n"                                   \
                           "Message: {}\n\n",                             \
                           #Expr, __FILE__, __LINE__, Msg);               \
            SCL_DEBUGBREAK();                                             \
        }                                                                 \
    }

    /**
     * Assertion function.
     * 
     * \param Expr - expression to be calculated.
     *               If 0 - assertion message showed in logger and debug break occures.
     * \param Msg - message to be displayed in case of assertion faield.
     * \return None.
     */
#   define SCL_ASSERT(Expr, Msg)                                          \
    {                                                                     \
        if (!(Expr))                                                      \
        {                                                                 \
            SCL_ERROR("Assertion Failed!\n"                               \
                      "Expr: {}\n"                                        \
                      "File: {}\n"                                        \
                      "Line: {}\n"                                        \
                      "Message: {}\n\n",                                  \
                      #Expr, __FILE__, __LINE__, Msg);                    \
            SCL_DEBUGBREAK();                                             \
        }                                                                 \
    }
#else /* !SCL_ASSERTION_ENABLED */
#   define SCL_CORE_ASSERT(expr, msg)
#   define SCL_ASSERT(expr, msg)
#endif
