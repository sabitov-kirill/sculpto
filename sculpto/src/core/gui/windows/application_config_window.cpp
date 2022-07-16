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
#include "core/render/primitives/shader.h"
#include "utilities/assets_manager/shaders_load.h"

void scl::application_config_window::Draw()
{
    ImGui::Begin("Application Configuration");
    {
        ImGui::Text("GUI configuration");
        ImGui::Checkbox("Use Dock space", &gui::IsDockspace);
        ImGui::Separator();

        vec4 clear_color = render_bridge::GetClearColor();
        bool is_wireframe = render_bridge::GetWireframeMode();
        bool is_vsync = render_bridge::GetVSync();
        ImGui::Text("Renderer configuration");
        if (ImGui::ColorEdit4("Clear Color", (float *)&clear_color))
            render_bridge::SetClearColor(clear_color);
        if (ImGui::Checkbox("Wireframe Mode", &is_wireframe)) render_bridge::SetWireframeMode(is_wireframe);
        if (ImGui::Checkbox("VSync", &is_vsync)) render_bridge::SetVSync(is_vsync);
        ImGui::NewLine();

        float w = ImGui::GetContentRegionAvail().x;
        ImGui::Text("Renderer build-in resources");

        if (ImGui::Button("Reload##shd1", { w * 0.2f, 0 })) assets_manager::UpdateShader(render_bridge::GetGaussianBlurPassShader()); ImGui::SameLine();
        ImGui::Text("Shader \"%s\"", render_bridge::GetGaussianBlurPassShader()->DebugName.c_str());

        if (ImGui::Button("Reload##shd2", { w * 0.2f, 0 })) assets_manager::UpdateShader(render_bridge::GetPhongGeometryShader()); ImGui::SameLine();
        ImGui::Text("Shader \"%s\"", render_bridge::GetPhongGeometryShader()->DebugName.c_str());

        if (ImGui::Button("Reload##shd3", { w * 0.2f, 0 })) assets_manager::UpdateShader(render_bridge::GetPhongLightingShader()); ImGui::SameLine();
        ImGui::Text("Shader \"%s\"", render_bridge::GetPhongLightingShader()->DebugName.c_str());

        if (ImGui::Button("Reload##shd4", { w * 0.2f, 0 })) assets_manager::UpdateShader(render_bridge::GetShadowPassShader()); ImGui::SameLine();
        ImGui::Text("Shader \"%s\"", render_bridge::GetShadowPassShader()->DebugName.c_str());

        if (ImGui::Button("Reload##shd5", { w * 0.2f, 0 })) assets_manager::UpdateShader(render_bridge::GetSingleColorMaterialShader()); ImGui::SameLine();
        ImGui::Text("Shader \"%s\"", render_bridge::GetSingleColorMaterialShader()->DebugName.c_str());

        if (ImGui::Button("Reload##shd6", { w * 0.2f, 0 })) assets_manager::UpdateShader(render_bridge::GetToneMappingPassShader()); ImGui::SameLine();
        ImGui::Text("Shader \"%s\"", render_bridge::GetToneMappingPassShader()->DebugName.c_str());
    }
    ImGui::End();
}
