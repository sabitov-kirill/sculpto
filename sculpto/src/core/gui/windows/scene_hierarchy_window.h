/*!****************************************************************//*!*
 * \file   editor_scene_hierarchy_window.h
 * \brief  Scene hierarchy GUI window class definition module.
 *
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "core/scene/scene.h"
#include "core/scene/scene_object.h"

namespace scl
{
    /*! Scene hierarchy GUI window class. */
    class scene_hierarchy_window
    {
    private: /*! Scene hierarchy window data. */
        scene *Scene {};
        std::function<void(scene_object)> OnObjectSelect {};
        scene_object SelectedObject {};

        char ObjectCreationNameTextBuffer[128] {};

    public:
        /*!*
         * Scene hierarchy window default constructor.
         * 
         * \param Scene - scene to display hierarchy tree of.
         * \param SelectedSceneObject - pointer to variable to set selecting itmes in.
         */
        scene_hierarchy_window(scene *Scene, const std::function<void(scene_object)> &OnObjectSelect);

        /*!*
         * Draw scene hierarchy window function.
         * 
         * \param None.
         * \return None.
         */
        void Draw();
    };
}
