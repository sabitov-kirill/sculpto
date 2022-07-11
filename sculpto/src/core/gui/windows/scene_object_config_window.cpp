/*****************************************************************//**
 * \file   scene_object_config_window.cpp
 * \brief  Scene object components configuration GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <entt.hpp>
#include <imgui.h>

#include "scene_object_config_window.h"
#include "core/components/components.h"
#include "core/resources/materials/material_phong.h"
#include "core/resources/materials/material_single_color.h"

void scl::scene_object_config_window::Draw()
{
    //     if (ImGui::Begin("Scene Object Configuration"))
    ImGui::Begin("Scene Object Configuration");
    {
        if (!ConfiguringObject.IsOk())
        {
            ImGui::End();
            return;
        }

        if (ConfiguringObject.HasComponent<object_name_component>())
        {
            auto &name = ConfiguringObject.GetComponent<object_name_component>();
            if (strcmp(NameComponentTextBuffer, name.Name.c_str()) != 0) strcpy_s(NameComponentTextBuffer, name.Name.c_str());
            if (ImGui::InputText("Name", NameComponentTextBuffer, 512))
                name.Name = std::string(NameComponentTextBuffer);
            ImGui::Separator();
        }

        if (ConfiguringObject.HasComponent<transform_component>())
        {
            auto &transform = ConfiguringObject.GetComponent<transform_component>();
            ImGui::Text("Transformation");
            if (ImGui::DragFloat3("Scale", transform.Scale, 0.05f, 0.005f, 10.0f))
                transform.InvalidateScale();
            if (ImGui::DragFloat3("Rotation", transform.Angles, 0.25f, -180.0f, 180.0f))
                transform.InvalidateRotation();
            if (ImGui::DragFloat3("Position", transform.Position, 0.1f, -50.0f, 50.0f))
                transform.InvalidatePosition();
            ImGui::Separator();
        }

        if (ConfiguringObject.HasComponent<native_script_component>())
        {
            auto &native_script = ConfiguringObject.GetComponent<native_script_component>();
            ImGui::Text("Native sript \"%s\"", native_script.Name.c_str());
            ImGui::Checkbox("Is active", &native_script.IsActive);
            ImGui::Separator();
        }

        if (ConfiguringObject.HasComponent<mesh_component>())
        {
            auto &mesh = ConfiguringObject.GetComponent<mesh_component>().Mesh;
            size_t submeshes_count = mesh->SubMeshes.size();
            if (submeshes_count != SubmeshSelectComboItems.size())
            {
                SubmeshSelectComboItems.resize(submeshes_count);
                for (int i = 0; i < submeshes_count; i++)
                    SubmeshSelectComboItems[i] = std::string("Mesh No.") + std::to_string(i);
            }

            ImGui::Text("Mesh");
            if (ImGui::BeginCombo("Select Submesh", SubmeshSelectComboItems[CurrentSubmeshIndex].c_str()))
            {
                for (int i = 0; i < submeshes_count; ++i)
                {
                    const bool isSelected = (CurrentSubmeshIndex == i);
                    if (ImGui::Selectable(SubmeshSelectComboItems[i].c_str(), isSelected))
                        CurrentSubmeshIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::Checkbox("Drawing", &mesh->IsDrawing);
            ImGui::Checkbox("Casting shadows", &mesh->IsCastingShadow);

            auto &submesh = mesh->SubMeshes[CurrentSubmeshIndex];
            ImGui::Text("Submesh Data");
            ImGui::Text("Vertice count: %d", submesh.VertexBuffer->GetCount());
            ImGui::Text("Indices count: %d", submesh.IndexBuffer->GetCount());
            auto material_ph = std::dynamic_pointer_cast<material_phong>(submesh.Material);
            if (material_ph)
            {

            }
            auto material_single = std::dynamic_pointer_cast<material_single_color>(submesh.Material);
            if (material_single)
            {
                vec3 color = material_single->GetColor();
                if (ImGui::DragFloat3("Color", color, 0.001, 0, 1))
                    material_single->SetColor(color);
            }

            ImGui::Separator();
        }
    }
    ImGui::End();
}
