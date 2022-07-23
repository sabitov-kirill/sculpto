/*!****************************************************************//*!*
 * \file   native_script_component.h
 * \brief  Runtime compiling script component (for scene objects behaviour) class implementation module.
 *
 * \author Sabitov Kirill
 * \date   21 July 2022
 *********************************************************************/

#pragma once

#if 0
#include <RuntimeObjectSystem/ObjectInterfacePerModule.h>
#include <RuntimeObjectSystem/IObject.h>

#include "core/scene/scene_object_behaviour.h"

#if defined(SCL_PLATFORM_WINDOWS)
#   define SCL_RUNTIME_LINKLIBRARY(library) RUNTIME_COMPILER_LINKLIBRARY(library)
#elif defined(SCL_PLATFORM_LINUX)
#   define SCL_RUNTIME_LINKLIBRARY(library) RUNTIME_COMPILER_LINKLIBRARY("-l" library)
#elif defined(SCL_PLATFORM_MACOS)
#   define SCL_RUNTIME_LINKLIBRARY(library) RUNTIME_COMPILER_LINKLIBRARY("-framework " library)
#endif

namespace scl
{
    /*! Runtime compiling script component (for scene objects behaviour) class. */
    class scene_object_runtime_behaviour: public scene_object_behaviour
    {
    public: /* Runtime compiling script component (for scene objects behaviour) data. */
        /*! RCCPP interface ids enum. */
        enum ScriptableID: InterfaceID
        {
            ID = IID_ENDInterfaceID,
        };
    };
}
#endif
