/*****************************************************************//**
 * \file   scene_object_config_window.cpp
 * \brief  Scene object components configuration GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include "scene_object_config_window.h"

scl::scene_object scl::scene_object_config_window::GetConfiguringObject() const { return ConfiguringObject; }

void scl::scene_object_config_window::SetConfiguringObject(scene_object ConfiguringObject)
{
    this->ConfiguringObject = ConfiguringObject;
    SubmeshSelectComboItems.clear();
    CurrentSubmeshIndex = 0;
    GBufferPreviewColorAttachment = 0;

    memset(PhongDiffuseTextureTextBuffer, 0, 128);
    memset(PhongSpecularTextureTextBuffer, 0, 128);
    memset(PhongEmissionTextureTextBuffer, 0, 128);
    memset(NormalMapTextureTextBuffer, 0, 128);
    memset(SingleColorTextureTextBuffer, 0, 128);

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
        if (ConfiguringObject.HasComponent<transform_component>())
            DrawComponentPanel<transform_component>();
        if (ConfiguringObject.HasComponent<native_script_component>())
            DrawComponentPanel<native_script_component>();
        if (ConfiguringObject.HasComponent<mesh_component>())
            DrawComponentPanel<mesh_component>();
        if (ConfiguringObject.HasComponent<camera_component>())
            DrawComponentPanel<camera_component>();
        if (ConfiguringObject.HasComponent<point_light_component>())
            DrawComponentPanel<point_light_component>();
        if (ConfiguringObject.HasComponent<directional_light_component>())
            DrawComponentPanel<directional_light_component>();
        if (ConfiguringObject.HasComponent<spot_light_component>())
            DrawComponentPanel<spot_light_component>();
    }
    ImGui::End();
}
