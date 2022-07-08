/*****************************************************************//**
 * \file   editor_layer_windows.cpp
 * \brief  Editor layer implementation module.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#include <imgui/imgui.h>

#include "editor_layer.h"

void editor::editor_layer::DrawDockSpace()
{
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpace Demo", &IsDockspace, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                application::Get().ShutDown();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

void editor::editor_layer::DrawRendererConfigWindow()
{
    ImGui::Begin("Renderer Configuration");
    {
        if (ImGui::ColorEdit4("Clear Color", (float *)&ClearColor))
            render_bridge::SetClearColor(ClearColor);
        if (ImGui::Checkbox("Wireframe Mode", &IsWireframe))
            render_bridge::SetWireframeMode(IsWireframe);
        ImGui::Checkbox("Use Dock space", &IsDockspace);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}

void editor::editor_layer::DrawSceneObjectConfigWindow()
{
    ImGui::Begin("Scene Object Configuration");
    ImGui::End();
}

void editor::editor_layer::DrawViewportWindow()
{
    ImGui::Begin("Viewport");
    {
        // Render framebuffer
        const ImVec2 window_size = ImGui::GetContentRegionAvail();
        const shared<frame_buffer> &main_frame_buffer = EditorScene->GetMainCamera().MainFrameBuffer;
        const frame_buffer_props &main_frame_buffer_props = main_frame_buffer->GetFrameBufferProps();

        // Call scene on event function to dispatche window resize callback
        if ((main_frame_buffer_props.Width != window_size.x || main_frame_buffer_props.Height != window_size.y) &&
            window_size.x > 0 && window_size.y > 0)
        {
            window_resize_event e { (int)window_size.x, (int)window_size.y };
            EditorScene->OnEvent(e);
        }

        ImGui::Image((ImTextureID)main_frame_buffer->GetColorAttachment()->GetHandle(),
                     ImVec2(window_size.x, window_size.y), {1, 1}, {0, 0});

        // ImGui::Image((ImTextureID)EditorScene->ShadowCaster->ShadowMap->GetDepthAttachment()->GetHandle(),
        //              ImVec2(window_size.x, window_size.y), {1, 1}, {0, 0});

        if (ImGui::IsItemHovered()) ImGui::CaptureMouseFromApp(false);
    }
    ImGui::End();
}
