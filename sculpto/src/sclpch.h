/*!****************************************************************//*!*
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
#include <filesystem>
#include <functional>
#include <exception>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <format>
#include <mutex>
#include <future>
#include <cstdarg>
#include <cstring>
#include <typeinfo>
#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>

/*! Detect SCL platform. */
#include "core/application/platform_detection.h"

#ifdef SCL_PLATFORM_WINDOWS
#   define NOMINMAX
#   include <windows.h>
#   include <windowsx.h>
#   include <wrl/client.h>
#endif /*! !SCL_PLATFORM_WINDOWS */

/*! Logging system. */
#include "core/application/log.h"

/*! Math module. */
#include "utilities/math/math.h"
