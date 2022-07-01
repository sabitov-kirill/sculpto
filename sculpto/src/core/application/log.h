/*****************************************************************//**
 * \file   log.h
 * \brief  Logging system definition module.
 * 
 * \author Sabitov Kirill
 * \date   23 June 2022
 *********************************************************************/

#pragma once

#include "utilities/memory/smart_ptr.h"
#include "utilities/logger/logger.h"

namespace scl
{
    /* Logging system class. */
    class log
    {
    private:
        /* Sculpto library core logger. */
        static shared<scl::logger> CoreLogger;
        /* Sculpto library client application logger. */
        static shared<scl::logger> ClientLogger;

    public:
        /**
         * Logger initialisation function.
         * 
         * \param None.
         * \return None.
         */
        static void Init();

        /* Sculpto library core logger getter function. */
        static shared<::scl::logger> &GetCoreLogger() { return CoreLogger; }
        /* Sculpto library client application logger getter function. */
        static shared<::scl::logger> &GetClientLogger() { return ClientLogger; }
    };
}

#define SCL_CORE_INFO(...) scl::log::GetCoreLogger()->Info(__VA_ARGS__)
#define SCL_CORE_SUCCES(...) scl::log::GetCoreLogger()->Success(__VA_ARGS__)
#define SCL_CORE_WARN(...) scl::log::GetCoreLogger()->Warn(__VA_ARGS__)
#define SCL_CORE_ERROR(...) scl::log::GetCoreLogger()->Error(__VA_ARGS__)

#define SCL_INFO(...) scl::log::GetClientLogger()->Info(__VA_ARGS__)
#define SCL_SUCCES(...) scl::log::GetClientLogger()->Success(__VA_ARGS__)
#define SCL_WARN(...) scl::log::GetClientLogger()->Warn(__VA_ARGS__)
#define SCL_ERROR(...) scl::log::GetClientLogger()->Error(__VA_ARGS__)