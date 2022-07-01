/*****************************************************************//**
 * \file   debug_memory_hook.h
 * \brief  Debug memory allocation controll hook definition module.
 *         Reports unfreed memory, if any.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

/* Detect SCL platform. */
#include "core/application/platform_detection.h"

#if SCL_DEBUG_MEMORY_ENABLED && defined(SCL_PLATFORM_WINDOWS)
#   ifndef NDEBUG 
#       define _CRTDBG_MAP_ALLOC
#       include  <crtdbg.h>
            /* Setting debug memory hooks. */
#           define SclSetDbgMemHooks() _CrtSetDbgFlag(                             \
                       _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF |           \
                       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)  \
                   )

            /* Dummy class definition. */
            static class __Dummy
            {
            public:
                /* Dummy class constructor.
                  * Initialises memory allocation debugging hook. */
                __Dummy() { SclSetDbgMemHooks(); }
            } __ooppss; /* Dummy class instance. */
#   endif /* !NDEBUG */
#   ifdef _DEBUG 
#       ifdef _CRTDBG_MAP_ALLOC 
#           define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#       endif /* !_CRTDBG_MAP_ALLOC */ 
#   endif /* !_DEBUG */
#endif /* !SCL_DEBUG_MEMORY_ENABLED */
