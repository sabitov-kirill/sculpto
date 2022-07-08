/*****************************************************************//**
 * \file   render_primitive.h
 * \brief  Render primitive abstract class definition module.
 * 
 * \author Sabitov Kirill
 * \date   06 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Render primitive abstract class */
    class render_primitive abstract
    {
    public:
        using handle = u32;

        /* Backend api render primitive hadnle getter function. */
        virtual handle GetHandle() const = 0;
    };
}
