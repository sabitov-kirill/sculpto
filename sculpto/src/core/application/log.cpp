/*!****************************************************************//*!*
 * \file   log.cpp
 * \brief  Logging system implementation module.
 * 
 * \author Sabitov Kirill
 * \date   23 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "log.h"

scl::shared<scl::logger> scl::log::CoreLogger;
scl::shared<scl::logger> scl::log::ClientLogger;

void scl::log::Init()
{
    CoreLogger = std::make_shared<scl::logger>("Core");
    ClientLogger = std::make_shared<scl::logger>("Client");

    SCL_CORE_SUCCES("Logging system initialized.");
}
