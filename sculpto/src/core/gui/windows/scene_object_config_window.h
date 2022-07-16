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
#include "core/resources/mesh.h"
#include "core/resources/topology/sphere.h"
#include "core/resources/materials/material_phong.h"
#include "core/resources/materials/material_single_color.h"
#include "utilities/assets_manager/shaders_load.h"
#include "utilities/assets_manager/textures_load.h"
#include "utilities/assets_manager/meshes_load.h"

namespace scl
{
    /* Scene object components configuration GUI window class. */
    class scene_object_config_window
    {
    private: /* Scene object configuration window data. */
        float PanelWidth {};

        static const char AddComponentComboItemsCames[7][64];
        std::vector<std::string> SubmeshSelectComboItems {};
        int CurrentSubmeshIndex {};
        int GBufferPreviewColorAttachment {};
        int CurrentAddingComponentIndex {};
        int CameraProjectionTypeIndex {};

        char NameTextBuffer[128] {};
        char LoadModelTextBuffer[128] {};
        char PhongDiffuseTextureTextBuffer[128] {};
        char PhongSpecularTextureTextBuffer[128] {};
        char PhongEmissionTextureTextBuffer[128] {};
        char NormalMapTextureTextBuffer[128] {};
        char SingleColorTextureTextBuffer[128] {};

        /* Currently configuring scene object. */
        scene_object ConfiguringObject {};

        /* Surrently confiduring scene object components flags, showing presence of specific components.
         * Updated each frame, because component can be changed outside of these window.
         */
        bool IsTransformComponent {};
        bool IsMeshComponent {};
        bool IsCameraComponent {};
        bool IsPointLightComponent {};
        bool IsDirectionalLightComponent {};
        bool IsSpotLightComponent {};

    public: /* Scene object configuration window data getter/setter functions. */
        /* Currently configuring object getter function. */
        scene_object GetConfiguringObject() const;
        /* Currently configuring object setter function. */
        void SetConfiguringObject(scene_object ConfiguringObject);

    private: /* Scene object configuration window methods. */
        /**
         * Reset stored data about configuring object function.
         * 
         * \param None.
         * \return None.
         */
        void ResetData();

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
            ImGui::Text("Transformation");
            if (DrawDeleteComponentButton<transform_component>()) return;
            ImGui::NewLine();

            auto &transform = ConfiguringObject.GetComponent<transform_component>();
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
            ImGui::Text("Native script");
            if (DrawDeleteComponentButton<native_script_component>()) return;
            ImGui::NewLine();

            auto &native_script = ConfiguringObject.GetComponent<native_script_component>();
            ImGui::Text("Name \"%s\"", native_script.Name.c_str());
            ImGui::Checkbox("Is active", &native_script.IsActive);
            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<mesh_component>()
        {
            ImGui::Text("Mesh");
            if (DrawDeleteComponentButton<mesh_component>()) return;
            ImGui::NewLine();

            auto &mesh = ConfiguringObject.GetComponent<mesh_component>().Mesh;
            if (mesh == nullptr) return;

            ImGui::InputText("##loadmoedl", LoadModelTextBuffer, 128); ImGui::SameLine();
            if (ImGui::Button("Load model")) mesh = assets_manager::LoadMeshes(std::string(LoadModelTextBuffer));

            if (ImGui::BeginCombo("Select Submesh", SubmeshSelectComboItems[CurrentSubmeshIndex].c_str()))
            {
                for (int i = 0; i < SubmeshSelectComboItems.size(); ++i)
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

            ImGui::SetNextItemWidth(PanelWidth * 0.8);
            ImGui::Text("Shader \"%s\"", submesh.Material->Shader->DebugName.c_str()); ImGui::SameLine(PanelWidth * 0.8f);
            if (ImGui::Button("Reload##shdmtl", { PanelWidth * 0.2f, 0 })) assets_manager::UpdateShader(submesh.Material->Shader);
            ImGui::NewLine();

            if (material_ph) {
                // Diffuse map
                ImGui::Text("Diffuse");
                if (material_ph->GetIsDiffuseMap()) {
                    const shared<texture_2d> &texture = material_ph->GetDiffuseMapTexture();
                    ImGui::Image((ImTextureID)texture->GetHandle(), { PanelWidth, PanelWidth * texture->GetHeight() / texture->GetWidth() }, { 0, 1 }, { 1, 0 });
                }
                ImGui::InputText("##newdiffmappath", PhongDiffuseTextureTextBuffer, 128); ImGui::SameLine();
                if (ImGui::Button("Load new texture")) material_ph->SetDiffuseMapTexture(assets_manager::LoadTexture(std::string(SingleColorTextureTextBuffer)));
                vec3 color = material_ph->GetDiffuse();
                if (ImGui::ColorEdit3("Diffuse", color)) material_ph->SetDiffuse(color);
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
                if (ImGui::ColorEdit3("Specular", color)) material_ph->SetSpecular(color);
                ImGui::NewLine();

                float shininess = material_ph->GetShininess();
                if (ImGui::DragFloat("Shininess", &shininess, 0.05, 0.001, 512)) material_ph->SetShininess(shininess);
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
            ImGui::Text("Camera");
            if (DrawDeleteComponentButton<camera_component>()) return;
            ImGui::NewLine();

            auto &camera_comp = ConfiguringObject.GetComponent<camera_component>();
            auto &camera = camera_comp.Camera;
            ImGui::Checkbox("Is primary", &camera_comp.IsPrimary);
            CameraProjectionTypeIndex = (int)camera.GetProjectionType();
            if (ImGui::Combo("Projection type", &CameraProjectionTypeIndex, "Orthographic\0Perspective"))
                camera.SetProjectionType((camera_projection_type)CameraProjectionTypeIndex);
            float fov = camera.GetFieldOfView();
            if (ImGui::DragFloat("Field of view", &fov, 0.001, 0.0001, 2)) camera.SetFieldOfView(fov);
            ImGui::NewLine();

            vec3 position = camera.GetPosition();
            if (ImGui::DragFloat3("Camera Position", position, 0.05, -100, 100)) camera.SetPosition(position);
            vec3 focus = camera.GetFocus();
            if (ImGui::DragFloat3("Focus point", focus, 0.05, -100, 100)) camera.SetFocus(focus);
            vec3 up = camera.GetUpDirection();
            if (ImGui::DragFloat3("Up direction", up, 0.001, -1, 1)) camera.SetUpDirection(up);
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
            ImGui::Text("Point Light");
            if (DrawDeleteComponentButton<point_light_component>()) return;
            ImGui::NewLine();

            auto &light = ConfiguringObject.GetComponent<point_light_component>();
            ImGui::ColorEdit3("Light color", light.Color);
            ImGui::SliderFloat("Light strength", &light.Strength, 0.001, 10);
            ImGui::NewLine();

            ImGui::DragFloat("Constant attitution", &light.Constant, 0.01, 0.001, 3);
            ImGui::DragFloat("Linear attitution", &light.Linear, 0.01, 0.001, 2);
            ImGui::DragFloat("Quadratic attitution", &light.Quadratic, 0.0001, 0.00000001, 2, "%.8f");

            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<directional_light_component>()
        {
            ImGui::Text("Directiona Light");
            if (DrawDeleteComponentButton<directional_light_component>()) return;
            ImGui::NewLine();

            auto &light = ConfiguringObject.GetComponent<directional_light_component>();
            ImGui::ColorEdit3("Light color", light.Color);
            ImGui::SliderFloat("Light strength", &light.Strength, 0.001, 10);
            ImGui::NewLine();

            bool is_shadows = light.GetIsShadow();
            if (ImGui::Checkbox("Is casting shadows", &is_shadows)) light.SetIsShadows(is_shadows);
            if (is_shadows)
            {
                float size = light.GetBoxSize(), depth = light.GetBoxDepth();
                if (ImGui::DragFloat("Projection box size", &size, 0.01, 5, 500)) light.SetBoxSize(size);
                if (ImGui::DragFloat("Projection box depth", &depth, 0.01, 5, 500)) light.SetBoxDepth(depth);
                ImGui::NewLine();

                auto &depth_texture = light.GetShadowMap()->GetDepthAttachment();
                int w = depth_texture->GetWidth(), h = depth_texture->GetHeight();
                ImGui::Text("Shadow map");
                if (ImGui::DragInt("Shadow map width", &w, 5, 100, 10000)) light.SetShadowMapWidth(w);
                if (ImGui::DragInt("Shadow map height", &h, 5, 100, 10000)) light.SetShadowMapHeight(h);
                ImGui::Image((ImTextureID)depth_texture->GetHandle(),
                             { PanelWidth, PanelWidth * h / w }, {0, 1}, {1, 0});
                ImGui::NewLine();
            }

            ImGui::Separator();
        }

        template <>
        void DrawComponentPanel<spot_light_component>()
        {

            ImGui::Text("Spot Light");
            if (DrawDeleteComponentButton<spot_light_component>()) return;
            ImGui::NewLine();

            auto &light = ConfiguringObject.GetComponent<spot_light_component>();
            ImGui::ColorEdit3("Light color", light.Color);
            ImGui::SliderFloat("Light strength", &light.Strength, 0.001, 10);
            ImGui::NewLine();

            float inner_cutoff = light.GetInnerCutoff();
            float outer_cutoff = light.GetOuterCutoff();
            if (ImGui::DragFloat("Inner cutoff angle", &inner_cutoff)) light.SetInnerCutoff(inner_cutoff);
            if (ImGui::DragFloat("Outer cutoff angle", &outer_cutoff)) light.SetOuterCutoff(outer_cutoff);

            ImGui::Separator();
        }

        void DrawAddComponentComboItem(int ComponentIndex)
        {
            const bool isSelected = (CurrentAddingComponentIndex == ComponentIndex);
            if (ImGui::Selectable(AddComponentComboItemsCames[ComponentIndex], isSelected))
                CurrentAddingComponentIndex = ComponentIndex;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }

        void DrawAddComponentPanel()
        {
            ImGui::SetNextItemWidth(PanelWidth * 0.8f);
            if (ImGui::BeginCombo("##adding_component_selection", AddComponentComboItemsCames[CurrentAddingComponentIndex]))
            {
                if (!IsTransformComponent)        DrawAddComponentComboItem(1);
                if (!IsMeshComponent)             DrawAddComponentComboItem(2);
                if (!IsCameraComponent)           DrawAddComponentComboItem(3);
                if (!IsPointLightComponent)       DrawAddComponentComboItem(4);
                if (!IsDirectionalLightComponent) DrawAddComponentComboItem(5);
                if (!IsSpotLightComponent)        DrawAddComponentComboItem(6);

                ImGui::EndCombo();
            }

            ImGui::SameLine();
            if (ImGui::Button("Add component", { PanelWidth * 0.2f, 0 }))
            {
                switch (CurrentAddingComponentIndex)
                {
                case 1: ConfiguringObject.AddComponent<transform_component>();         break;
                case 2: ConfiguringObject.AddComponent<mesh_component>(mesh::Create(topology::sphere(vec3 { 0 }, 1, 20),
                                                                                    material_single_color::Create(vec3 { 1 })));
                                                                                       break;
                case 3: ConfiguringObject.AddComponent<camera_component>();            break;
                case 4: ConfiguringObject.AddComponent<point_light_component>();       break;
                case 5: ConfiguringObject.AddComponent<directional_light_component>(); break;
                case 6: ConfiguringObject.AddComponent<spot_light_component>();        break;
                }
                ResetData();
            }
        }

        template <typename Tcomponent>
        bool DrawDeleteComponentButton()
        {
            static std::string button_name = std::string("Delete##") + typeid(Tcomponent).name();

            ImGui::SameLine(PanelWidth * 0.8f);
            if (ImGui::Button(button_name.c_str(), { PanelWidth * 0.2f, 0 }))
            {
                ConfiguringObject.RemovetComponent<Tcomponent>();
                ResetData();
                return true;
            }
            return false;
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
