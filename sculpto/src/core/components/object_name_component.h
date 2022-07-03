/*****************************************************************//**
 * \file   object_name_component.h
 * \brief  
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

#define SCL_MAKE_NAME_FROM_TYPE(type) #type

namespace scl
{
    struct object_name_component
    {
        std::string Name;

        object_name_component(const std::string &Name) : Name(Name) {}
        object_name_component(const object_name_component &Other) = default;
        ~object_name_component() = default;
    };
}
