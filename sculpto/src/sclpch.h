/*****************************************************************//**
 * \file   sclpch.h
 * \brief  Sculpto library prehompiled header.
 *         Defines common definitions, includes commonly used modules.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <exception>
#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <set>

#ifdef SCL_PLATFORM_WINDOWS
#   include <windows.h>
#   include <windowsx.h>
#endif /* !SCL_PLATFORM_WINDOWS */

/* Common macros. */
#include "utilities/macros/debug_memory_hook.h"
#include "utilities/macros/assert.h"
#include "utilities/macros/api_build.h"
#include "utilities/macros/throw_if_failed.h"

/* Math module. */
#include "utilities/math/math.h"

/* Standart types definition. */
namespace scl
{
    /* Atomic types. */
    using i8 = __int8;
    using u8 = unsigned __int8;
    using i16 = __int16;
    using u16 = unsigned __int16;
    using i32 = __int32;
    using u32 = unsigned __int32;
    using i64 = __int64;
    using u64 = unsigned __int64;

    /* Math module generic types. */
    using vec2 = math::vec2<float>;
    using vec3 = math::vec3<float>;
    using vec4 = math::vec4<float>;
    using ivec2 = math::vec2<int>;
    using ivec3 = math::vec3<int>;
    using ivec4 = math::vec4<int>;
    using uvec2 = math::vec2<u32>;
    using uvec3 = math::vec3<u32>;
    using uvec4 = math::vec4<u32>;
    using matr4 = math::vec4<float>;
    using camera = math::camera<float>;

    /* Windows platform specific types */
#ifdef SCL_PLATFORM_WINDOWS
    using window_handle = HWND;
    using instance_handle = HINSTANCE;
    using timer_handle = u32;
    using dc_handle = HDC;
    using rect = RECT;
#endif /* !SCL_PLATFORM_WINDOWS */
}
