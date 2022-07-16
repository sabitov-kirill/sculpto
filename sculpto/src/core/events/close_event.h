/*****************************************************************//**
 * \file   window_event.h
 * \brief  Window update specific events implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#pragma once

#include "utilities/event/event.h"

namespace scl
{
    /* Window close event class. */
    class close_event: public event
    {
        SCL_MAKE_EVENT(WindowClose);
    };
}
