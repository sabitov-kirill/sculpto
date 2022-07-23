/*!****************************************************************//*!*
 * \file   sclpch.h
 * \brief  Sculpto library prehompiled header.
 *         Defines common definitions, includes commonly used modules.
 *
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

/*! Disable C4151 due to the fact that it is supposed
 * to compile all projects with one compiler,
 * and becaouse of that, side effects will be absent.
 */
#pragma warning(disable : 4251)

/*! Common Utilities. */
#include "utilities/macros/debug_memory_hook.h"
#include "utilities/macros/assert.h"
#include "utilities/macros/throw_if_failed.h"
#include "utilities/memory/smart_ptr.h"
#include "utilities/string/fixed_string.h"
#include "utilities/string/string_id.h"
#include "utilities/event/event_dispatcher.h"

#include "core/application/log.h"

#define SCL_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

/*! Standart types definition. */
namespace scl
{
    /*! Math module generic types. */
    using degrees    = math::degrees<float>;
    using radians    = math::radians<float>;
    using vec2       = math::vec2<float>;
    using vec3       = math::vec3<float>;
    using vec4       = math::vec4<float>;
    using ivec2      = math::vec2<int>;
    using ivec3      = math::vec3<int>;
    using ivec4      = math::vec4<int>;
    using uvec2      = math::vec2<u32>;
    using uvec3      = math::vec3<u32>;
    using uvec4      = math::vec4<u32>;
    using matr3      = math::matr3<float>;
    using matr3_data = math::matr3_data<float>;
    using matr4      = math::matr4<float>;
    using matr4_data = math::matr4_data<float>;

    /*! Windows platform specific types */
#ifdef SCL_PLATFORM_WINDOWS
    using window_handle = HWND;
#else
    using window_handle = int;
#endif /*! !SCL_PLATFORM_WINDOWS */
}
