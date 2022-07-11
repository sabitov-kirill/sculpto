/*****************************************************************//**
 * \file   scene_config_window.h
 * \brief  Scene configuration GUI window class definition module.
 *
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "core/scene/scene.h"

namespace scl
{
    /* Scene hierarchy GUI window class. */
    class scene_config_window
    {
    private: /* Scene hierarchy window data. */
        scene *Scene {};

    public:
        /**
         * Scene hierarchy window default constructor.
         *
         * \param Scene - scene to display hierarchy tree of.
         * \param SelectedSceneObject - pointer to variable to set selecting itmes in.
         */
        scene_config_window(scene *Scene);

        /**
         * Draw scene hierarchy window function.
         *
         * \param None.
         * \return None.
         */
        void Draw();
    };
}

