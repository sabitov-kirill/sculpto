/*****************************************************************//**
 * \file   scene_object_config_window.cpp
 * \brief  Scene object components configuration GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "scene_object_config_window.h"

const char scl::scene_object_config_window::AddComponentComboItemsCames[7][64] = {
    "Select component",
    "Transform Component",
    "Mesh Component",
    "Camera Component",
    "Point Light Component",
    "Directional Light Component",
    "Spot Light Component"
};

scl::scene_object scl::scene_object_config_window::GetConfiguringObject() const { return ConfiguringObject; }

void scl::scene_object_config_window::SetConfiguringObject(scene_object ConfiguringObject)
{
    this->ConfiguringObject = ConfiguringObject;
    ResetData();
}

void scl::scene_object_config_window::ResetData()
{
    CurrentSubmeshIndex = 0;
    GBufferPreviewColorAttachment = 0;
    CurrentAddingComponentIndex = 0;
    CameraProjectionTypeIndex = 0;

    memset(NameTextBuffer, 0, 128);
    memset(LoadModelTextBuffer, 0, 128);
    memset(PhongDiffuseTextureTextBuffer, 0, 128);
    memset(PhongSpecularTextureTextBuffer, 0, 128);
    memset(PhongEmissionTextureTextBuffer, 0, 128);
    memset(NormalMapTextureTextBuffer, 0, 128);
    memset(SingleColorTextureTextBuffer, 0, 128);

    SubmeshSelectComboItems.clear();
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
    }
}

void scl::scene_object_config_window::Draw()
{
    ImGui::Begin("Scene Object Configuration");
    {
        if (!ConfiguringObject.IsOk())
        {
            ImGui::End();
            return;
        }

        auto window_size = ImGui::GetWindowSize();
        PanelWidth = (float)window_size.x - 15;

        if (ConfiguringObject.HasComponent<object_name_component>())
            DrawComponentPanel<object_name_component>();
        if ((IsTransformComponent = ConfiguringObject.HasComponent<transform_component>()))
            DrawComponentPanel<transform_component>();
        if (ConfiguringObject.HasComponent<native_script_component>())
            DrawComponentPanel<native_script_component>();
        if ((IsMeshComponent = ConfiguringObject.HasComponent<mesh_component>()))
            DrawComponentPanel<mesh_component>();
        if ((IsCameraComponent = ConfiguringObject.HasComponent<camera_component>()))
            DrawComponentPanel<camera_component>();
        if ((IsPointLightComponent = ConfiguringObject.HasComponent<point_light_component>()))
            DrawComponentPanel<point_light_component>();
        if ((IsDirectionalLightComponent = ConfiguringObject.HasComponent<directional_light_component>()))
            DrawComponentPanel<directional_light_component>();
        if ((IsSpotLightComponent = ConfiguringObject.HasComponent<spot_light_component>()))
            DrawComponentPanel<spot_light_component>();
        ImGui::NewLine();

        DrawAddComponentPanel();
    }
    ImGui::End();
}
