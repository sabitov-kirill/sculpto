/*****************************************************************//**
 * \file   scene_object_config_window.h
 * \brief  Scene object components configuration GUI window class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "core/scene/scene_object.h"

namespace scl
{
    /* Scene object components configuration GUI window class. */
    class scene_object_config_window
    {
    private:
        char NameComponentTextBuffer[512] {};
        std::vector<std::string> SubmeshSelectComboItems {};
        int CurrentSubmeshIndex {};

    public:
        /* Currently configuring scene object. */
        scene_object ConfiguringObject {};

        /* Scene object components configuration GUI window default constructor. */
        scene_object_config_window() = default;

        /**
         * Draw scene object configuration window function.
         * 
         * \param None.
         * \return None.
         */
        void Draw();
    };
}
