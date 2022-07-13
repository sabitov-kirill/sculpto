/*****************************************************************//**
 * \file   scene_object_config_window.h
 * \brief  Scene object components configuration GUI window class declaration module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include <entt.hpp>
#include <imgui.h>

#include "core/scene/scene_object.h"
#include "core/components/components.h"
#include "core/resources/materials/material_phong.h"
#include "core/resources/materials/material_single_color.h"
#include "utilities/assets_manager/shaders_load.h"
#include "utilities/assets_manager/textures_load.h"

namespace scl
{
    /* Scene object components configuration GUI window class. */
    class scene_object_config_window
    {
    private: /* Scene object configuration window data. */
        float                    PanelWidth {};

        std::vector<std::string> SubmeshSelectComboItems {};
        int                      CurrentSubmeshIndex {};
        int                      GBufferPreviewColorAttachment {};

        char                     NameTextBuffer[128] {};
        char                     PhongDiffuseTextureTextBuffer[128] {};
        char                     PhongSpecularTextureTextBuffer[128] {};
        char                     PhongEmissionTextureTextBuffer[128] {};
        char                     NormalMapTextureTextBuffer[128] {};
        char                     SingleColorTextureTextBuffer[128] {};

        /* Currently configuring scene object. */
        scene_object ConfiguringObject {};

    public: /* Scene object configuration window data getter/setter functions. */
        /* Currently configuring object getter function. */
        scene_object GetConfiguringObject() const;
        /* Currently configuring object setter function. */
        void SetConfiguringObject(scene_object ConfiguringObject);

    private: /* Scene object configuration window methods. */
        /**
         * Draw GUI panel for specific component of currently editing object function.
         * 
         * \param None.
         * \return None.
         */
        template <typename Tcomponent>
        void DrawComponentPanel() {}

        template <>
        void DrawComponentPanel<object_name_component>()
        {
            auto &name = ConfiguringObject.GetComponent<object_name_component>();
            if (strcmp(NameTextBuffer, name.Name.c_str()) != 0) strcpy_s(NameTextBuffer, name.Name.c_str());
            if (ImGui::InputText("Name", NameTextBuffer, 128))
                name.Name = std::string(NameTextBuffer);
            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<transform_component>()
        {
            auto &transform = ConfiguringObject.GetComponent<transform_component>();
            ImGui::Text("Transformation");
            ImGui::NewLine();

            if (ImGui::DragFloat3("Scale", transform.Scale, 0.05f, 0.005f, 10.0f))
                transform.InvalidateScale();
            if (ImGui::DragFloat3("Rotation", transform.Angles, 0.25f, -180.0f, 180.0f))
                transform.InvalidateRotation();
            if (ImGui::DragFloat3("Position", transform.Position, 0.1f, -50.0f, 50.0f))
                transform.InvalidatePosition();
            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<native_script_component>()
        {
            auto &native_script = ConfiguringObject.GetComponent<native_script_component>();
            ImGui::Text("Native script \"%s\"", native_script.Name.c_str());
            ImGui::NewLine();

            ImGui::Checkbox("Is active", &native_script.IsActive);
            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<mesh_component>()
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
            ImGui::NewLine();

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
            ImGui::NewLine();

            auto &submesh = mesh->SubMeshes[CurrentSubmeshIndex];
            ImGui::Text("Submesh Data");
            ImGui::Text("Vertice count: %d", submesh.VertexBuffer->GetCount());
            ImGui::Text("Indices count: %d", submesh.IndexBuffer->GetCount());
            ImGui::NewLine();

            auto material_ph = std::dynamic_pointer_cast<material_phong>(submesh.Material);
            auto material_single = std::dynamic_pointer_cast<material_single_color>(submesh.Material);
            ImGui::Text("Shader %s", submesh.Material->Shader->DebugName.c_str());
            if (ImGui::Button("Reload Shader")) assets_manager::UpdateShader(submesh.Material->Shader);
            ImGui::NewLine();

            if (material_ph) {
                // Diffuse map
                ImGui::Text("Diffuse");
                if (material_ph->GetIsDiffuseMap()) {
                    const shared<texture_2d> &texture = material_ph->GetDiffuseMapTexture();
                    ImGui::Image((ImTextureID)texture->GetHandle(), { PanelWidth, PanelWidth * texture->GetHeight() / texture->GetWidth() }, {0, 1}, {1, 0});
                }
                ImGui::InputText("##newdiffmappath", PhongDiffuseTextureTextBuffer, 128); ImGui::SameLine();
                if (ImGui::Button("Load new texture")) material_ph->SetDiffuseMapTexture(assets_manager::LoadTexture(std::string(SingleColorTextureTextBuffer)));
                vec3 color = material_ph->GetDiffuse();
                if (ImGui::ColorEdit3("Color", color)) material_ph->SetDiffuse(color);
                ImGui::NewLine();

                // Specular map
                ImGui::Text("Specular");
                if (material_ph->GetIsSpecularMap()) {
                    const shared<texture_2d> &texture = material_ph->GetSpecularMapTexture();
                    ImGui::Image((ImTextureID)texture->GetHandle(), { PanelWidth, PanelWidth * texture->GetHeight() / texture->GetWidth() }, { 0, 1 }, { 1, 0 });
                }
                ImGui::InputText("##newspecmappath", PhongSpecularTextureTextBuffer, 128); ImGui::SameLine();
                if (ImGui::Button("Load new texture")) material_ph->SetSpecularMapTexture(assets_manager::LoadTexture(std::string(SingleColorTextureTextBuffer)));
                color = material_ph->GetSpecular();
                if (ImGui::ColorEdit3("Color", color)) material_ph->SetSpecular(color);
                ImGui::NewLine();

                // Emission map
                ImGui::Text("Emission map");
                if (material_ph->GetIsEmissionMap()) {
                    const shared<texture_2d> &texture = material_ph->GetEmissionMapTexture();
                    ImGui::Image((ImTextureID)texture->GetHandle(), { PanelWidth, PanelWidth * texture->GetHeight() / texture->GetWidth() }, { 0, 1 }, { 1, 0 });
                }
                ImGui::InputText("##newemissionpath", PhongEmissionTextureTextBuffer, 128); ImGui::SameLine();
                if (ImGui::Button("Load new texture")) material_ph->SetEmissionMapTexture(assets_manager::LoadTexture(std::string(SingleColorTextureTextBuffer)));
                ImGui::NewLine();

                // Normal map
                ImGui::Text("Normal map");
                if (material_ph->GetIsNormalMap()) {
                    const shared<texture_2d> &texture = material_ph->GetNormalMapTexture();
                    ImGui::Image((ImTextureID)texture->GetHandle(), { PanelWidth, PanelWidth * texture->GetHeight() / texture->GetWidth() }, { 0, 1 }, { 1, 0 });
                }
                ImGui::InputText("##newnmappath", NormalMapTextureTextBuffer, 128); ImGui::SameLine();
                if (ImGui::Button("Load new texture")) material_ph->SetNormalMapTexture(assets_manager::LoadTexture(std::string(SingleColorTextureTextBuffer)));

            } else if (material_single) {
                if (material_single->GetIsTexture()) {
                    const shared<texture_2d> &texture = material_single->GetTexture();
                    ImGui::Text("Texture");
                    ImGui::Image((ImTextureID)texture->GetHandle(), { PanelWidth, PanelWidth * texture->GetHeight() / texture->GetWidth() }, { 0, 1 }, { 1, 0 });
                }
                ImGui::InputText("##newtexturepath", SingleColorTextureTextBuffer, 128); ImGui::SameLine();
                if (ImGui::Button("Load new texture")) material_single->SetTexture(assets_manager::LoadTexture(std::string(SingleColorTextureTextBuffer)));
                vec3 color = material_single->GetColor();
                if (ImGui::ColorEdit3("Color", color, 0.001)) material_single->SetColor(color);
            }
            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<camera_component>()
        {
            auto &camera = ConfiguringObject.GetComponent<camera_component>().Camera;
            auto window_size = ImGui::GetWindowSize();
            window_size.x -= 15, window_size.y -= 15;

            ImGui::Text("Camera");
            ImGui::NewLine();

            ImGui::Combo("GBuffer", &GBufferPreviewColorAttachment, "None\0Position\0Normals\0Color\0Diffuse\0Specular\0Shininess");
            if (GBufferPreviewColorAttachment > 0)
                ImGui::Image((ImTextureID)camera.GetGBuffer()->GetColorAttachment(GBufferPreviewColorAttachment - 1)->GetHandle(),
                             { PanelWidth, PanelWidth * camera.GetViewportHeight() / camera.GetViewportWidth() }, { 0, 1 }, { 1, 0 });
            ImGui::NewLine();

            ImGui::Text("Camera effects");
            ImGui::Checkbox("HDR mode", &camera.Effects.HDR);
            if (camera.Effects.HDR)
            {
                ImGui::SliderFloat("Exposure level", &camera.Effects.Exposure, 0.01, 10);
                ImGui::NewLine();

                ImGui::Checkbox("Bloom effect", &camera.Effects.Bloom);
                if (camera.Effects.Bloom) ImGui::SliderInt("Bloom amount", &camera.Effects.BloomAmount, 4, 12);
            }

            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<point_light_component>()
        {
            auto &light = ConfiguringObject.GetComponent<point_light_component>();

            ImGui::Text("Point Light");
            ImGui::ColorEdit3("Light color", light.Color);
            ImGui::DragFloat("Constant attitution", &light.Constant, 0.01, 0.001, 3);
            ImGui::DragFloat("Linear attitution", &light.Linear, 0.01, 0.001, 2);
            ImGui::DragFloat("Quadratic attitution", &light.Quadratic, 0.0001, 0.00000001, 2, "%.8f");

            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<directional_light_component>()
        {
            auto &light = ConfiguringObject.GetComponent<directional_light_component>();

            ImGui::Text("Directiona Light");
            ImGui::NewLine();

            ImGui::Text("Shadow map");
            ImGui::Image((ImTextureID)light.ShadowMap->GetDepthAttachment()->GetHandle(),
                         { PanelWidth, PanelWidth }, { 0, 1 }, { 1, 0 });
            ImGui::NewLine();

            ImGui::DragFloat3("Light direction", light.Direction, 0.01, 0, 1);
            ImGui::ColorEdit3("Light color", light.Color);

            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<spot_light_component>()
        {
            auto &light = ConfiguringObject.GetComponent<spot_light_component>();

            ImGui::Text("Spot Light");
            ImGui::NewLine();

            ImGui::DragFloat3("Light direction", light.Direction, 0.01, 0, 1);
            ImGui::ColorEdit3("Light color", light.Color);
            ImGui::NewLine();

            float inner_cutoff = light.GetInnerCutoff();
            float outer_cutoff = light.GetOuterCutoff();
            if (ImGui::DragFloat("Inner cutoff angle", &inner_cutoff)) light.SetInnerCutoff(inner_cutoff);
            if (ImGui::DragFloat("Outer cutoff angle", &outer_cutoff)) light.SetOuterCutoff(outer_cutoff);

            ImGui::Separator();
        }

    public:

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
