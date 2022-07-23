/*!****************************************************************//*!*
 * \file   event_dispatcher.cpp
 * \brief  
 * 
 * \author Sabitov Kirill
 * \date   13 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "event_dispatcher.h"

std::map<scl::string_id, scl::event_dispatcher::callback_list> scl::event_dispatcher::EventHandlers {};
