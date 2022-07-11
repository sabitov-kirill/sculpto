/*****************************************************************//**
 * \file   application_config_window.cpp
 * \brief  Application configuration GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>

#include "application_config_window.h"
#include "core/application/application.h"
#include "core/render/render_bridge.h"

scl::application_config_window::application_config_window()
{
    render_bridge::SetClearColor(ClearColor);
    render_bridge::SetWireframeMode(IsWireframe);
}

void scl::application_config_window::Draw()
{
    ImGui::Begin("Application Configuration");
    {
        ImGui::Text("Rendering configuration");
        if (ImGui::ColorEdit4("Clear Color", (float *)&ClearColor))
            render_bridge::SetClearColor(ClearColor);
        if (ImGui::Checkbox("Wireframe Mode", &IsWireframe))
            render_bridge::SetWireframeMode(IsWireframe);
        ImGui::Separator();

        ImGui::Text("GUI data");
        ImGui::Checkbox("Use Dock space", &application::Get().GUILayer->IsDockspace);
    }
    ImGui::End();
}
