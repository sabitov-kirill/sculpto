/*****************************************************************//**
 * \file   scene_hierarchy_window.cpp
 * \brief  Scene hierarchy GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>

#include "scene_hierarchy_window.h"
#include "core/components/components.h"

scl::scene_hierarchy_window::scene_hierarchy_window(scene *Scene, const std::function<void(scene_object)> &OnObjectSelect) :
    Scene(Scene), OnObjectSelect(OnObjectSelect) {}

void scl::scene_hierarchy_window::Draw()
{
    if (Scene == nullptr) return;

    ImGui::Begin("Scene Hierarchy");
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick
        | ImGuiTreeNodeFlags_SpanAvailWidth;
    for (auto &&[entity, tag] : Scene->Registry.view<object_name_component>().each())
    {
        bool is_open = ImGui::TreeNodeEx((void *)(u64)(u32)entity,
                                         node_flags | (ImGuiTreeNodeFlags_Selected * (entity == SelectedObject)),
                                         "%s", tag.Name.c_str());
        if (ImGui::IsItemClicked()) {
            SelectedObject = scene_object { entity, Scene };
            OnObjectSelect(SelectedObject);
        }
        if (is_open) ImGui::TreePop();
    }

    float window_width = ImGui::GetWindowSize().x;
    ImGui::SetNextItemWidth(window_width * 0.6f);
    ImGui::InputText("##adding_object_name", ObjectCreationNameTextBuffer, 128);
    ImGui::SameLine(window_width * 0.65f);
    if (ImGui::Button("Add new object", { window_width * 0.35f, 0 }))
        Scene->CreateObject(std::string(ObjectCreationNameTextBuffer)), memset(ObjectCreationNameTextBuffer, 0, 128);
    ImGui::End();
}
