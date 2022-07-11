/*****************************************************************//**
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
    ImGui::DragFloat3("Enviroment ambient", Scene->EnviromentAmbient, 0.05f, 0, 1);
    bool IsHDR = Scene->GetIsHDR();
    if (ImGui::Checkbox("HDR", &IsHDR)) Scene->SetIsHDR(IsHDR);
    bool IsBloom = Scene->GetIsBloom();
    if (ImGui::Checkbox("Bloom", &IsBloom)) Scene->SetIsBloom(IsBloom);
    ImGui::DragFloat("Exposure level", &Scene->Exposure, 0.005f, 0.001f, 10);
    ImGui::End();
}
