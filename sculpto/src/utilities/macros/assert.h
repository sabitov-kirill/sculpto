/*****************************************************************//**
 * \file   assert.h
 * \brief  Assertion macros definition module.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

 /* C Preprocessor sh*t for making from macro expression string. */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifdef SCL_ASSERTION_ENABLED
#   include <windows.h>
/* Asserton */
#   define SCL_ASSERT(expr)                                        \
        if (expr) {}                                               \
        else                                                       \
        {                                                          \
            MessageBox(nullptr, "File: " STR(__FILE__)             \
                                "\n\nLine: " STR(__LINE__),        \
                       "Assertion failed!", MB_ICONERROR | MB_OK); \
            exit(239);                                             \
        }
#else /* !SCL_ASSERTION_ENABLED */
#   define ASSERT(expr)
#endif
