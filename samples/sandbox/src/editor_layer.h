/*****************************************************************//**
 * \file   editor_layer.h
 * \brief  Editor layer definition module.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include <sculpto.h>
using namespace scl;

namespace editor
{
    class editor_layer: public layer
    {
    private:
        scl::vec4 ClearColor {};
        bool IsWireframe {};
        bool IsDockspace { true };

        shared<scene> EditorScene;

        void DrawDockSpace();
        void DrawRendererConfigWindow();
        void DrawSceneObjectConfigWindow();
        void DrawViewportWindow();

    public:
        editor_layer() = default;
        ~editor_layer() = default;

        void OnInit() override;
        void OnUpdate(float DeltaTime) override;
        void OnGuiRender() override;
        void OnEvent(event &Event) override;
    };

}
