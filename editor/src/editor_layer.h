/*****************************************************************//**
 * \file   editor_layer.h
 * \brief  Editor layer definition module.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include <sculpto.h>

namespace scl::editor
{
    class editor_layer: public layer
    {
    private:
        shared<scene> EditorScene {};
        shared<application_config_window> ConfigWindow {};
        shared<viewport_window> MainViewportWindow {};
        shared<profiller_window> ProfillerWindow {};
        shared<scene_config_window> SceneConfigWindow {};
        shared<scene_hierarchy_window> SceneHierarchyWindow {};
        shared<scene_object_config_window> SceneObjectConfigWindow {};

    public:
        editor_layer() = default;
        ~editor_layer() = default;

        void OnInit() override;
        void OnUpdate(float DeltaTime) override;
        void OnGuiRender() override;
    };
}
