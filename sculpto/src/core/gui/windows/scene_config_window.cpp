/*!****************************************************************//*!*
 * \file   scene_config_window.cpp
 * \brief  Scene configuration GUI window class definition module.
 * 
 * \author Sabitov Kirill
 * \date   10 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>

#include "scene_config_window.h"

scl::scene_config_window::scene_config_window(scene *Scene) :
    Scene(Scene) {}

void scl::scene_config_window::Draw()
{
    ImGui::Begin("Scene Consfiguration");
    ImGui::End();
}
