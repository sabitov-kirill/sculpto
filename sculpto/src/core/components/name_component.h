/*!****************************************************************//*!*
 * \file   name_component.h
 * \brief  Scene object name component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

#define SCL_MAKE_NAME_FROM_TYPE(type) #type

namespace scl
{
    struct name_component
    {
        std::string Name;

        name_component(const std::string &Name) : Name(Name) {}
        name_component(const name_component &Other) = default;
        ~name_component() = default;

        operator const std::string &() { return Name; }
        operator std::string &() { return Name; }
    };
}
