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

scl::scene_hierarchy_window::scene_hierarchy_window(scene *Scene, scene_object &SelectedSceneObject) :
    Scene(Scene), SelectedSceneObject(SelectedSceneObject) {}

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
                                         node_flags | (ImGuiTreeNodeFlags_Selected * (entity == SelectedSceneObject)),
                                         "%s", tag.Name.c_str());
        if (ImGui::IsItemClicked()) SelectedSceneObject = scene_object { entity, Scene };
        if (is_open) ImGui::TreePop();
    }
    ImGui::End();
}
