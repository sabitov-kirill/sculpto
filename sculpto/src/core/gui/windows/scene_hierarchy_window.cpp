/*!****************************************************************//*!*
 * \file   scene_hierarchy_window.cpp
 * \brief  Scene hierarchy GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>
#include <imgui_internal.h>

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
    for (auto &&[entity, tag] : Scene->Registry.view<name_component>().each())
    {
        if (ImGui::Selectable(tag.Name.c_str(), entity == SelectedObject, ImGuiSelectableFlags_None,
                              { entity == SelectedObject ? ImGui::GetWindowWidth() * 0.6f - 10 : 0, 20 }))
        {
            SelectedObject = scene_object { entity, Scene };
            OnObjectSelect(SelectedObject);
        }

        if (entity == SelectedObject)
        {
            ImGui::SameLine(ImGui::GetWindowWidth() * 0.6f + 5);
            if (ImGui::Button("Delete##delete_object", { ImGui::GetWindowWidth() * 0.4f - 10, 0 }))
                Scene->RemoveObject(SelectedObject);
        }
    }

    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6f - 5);
    ImGui::InputText("##adding_object_name", ObjectCreationNameTextBuffer, 128);
    ImGui::SameLine(ImGui::GetWindowWidth() * 0.6f + 5);
    if (ImGui::Button("Add new object", { ImGui::GetWindowWidth() * 0.4f - 10, 0 }))
        Scene->CreateObject(std::string(ObjectCreationNameTextBuffer)), memset(ObjectCreationNameTextBuffer, 0, 128);
    ImGui::End();
}
