/*****************************************************************//**
 * \file   main.cpp
 * \brief  Sculpto sandbox program entry point.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include <imgui/imgui.h>
#include "sandbox_app.h"
using namespace scl;

void test_gui_layer::OnInit()
{
    topology::sphere sphere(vec3(0), 1.5, 20);
    topology::grid quad(3, 3);
    topology::cube cube(vec3(0), vec3(1));
    shared<vertex_buffer> VBO = vertex_buffer::Create(cube.GetVertices().data(), cube.GetVertices().size(), vertex::GetVertexLayout());
    shared<index_buffer> IBO = index_buffer::Create((u32 *)cube.GetIndices().data(), cube.GetIndices().size());
    VAO = mesh::Create(quad.GetType(), VBO, IBO);
    Texture = texture_2d::Create("assets/images/02.jpg");
    ShaderProgram = shader_program::Create({
        { shader_type::VERTEX, "assets/shaders/Test.vert.glsl", false },
        { shader_type::PIXEL,  "assets/shaders/Test.frag.glsl", false },
    }, "Test rendering shader");

    frame_buffer_props FbProps {};
    FbProps.Width = 500;
    FbProps.Height = 280;
    FrameBuffer = frame_buffer::Create(FbProps);

    // application::Get().SetGuiEnabled(false);
}

void test_gui_layer::OnResponse()
{
    static float delta = 0;
    const timer &app_timer = application::Get().GetTimer();
    if (delta > 1)
    {
        SCL_INFO("FPS: {}", app_timer.GetFps());
        delta = 0;
    }
    delta += app_timer.GetDeltaTime();
}

void test_gui_layer::OnRender()
{
    FrameBuffer->Bind();

    render_adapter &adapter = render_adapter::Get();
    adapter.RenderFrame();
    float time = scl::application::Get().GetTimer().GetTime();
    ShaderProgram->Bind();
    ShaderProgram->SetFloat("Time", time);
    ShaderProgram->SetMatr4("MatrWVP", matr4::RotateX(ObjectRotationX) *
                                       matr4::RotateY(ObjectRotationY) *
                                       matr4::RotateZ(ObjectRotationZ) *
                                       matr4::Translate(ObjectPosition) *
                                       matr4::Scale(vec3(ObjectSize)) *
                                       render_adapter::Get().GetCamera().VP);

    Texture->Bind(0);
    VAO->Bind();
    render_adapter::Get().GetRenderer().DrawVerices(VAO);
    VAO->Unbind();

    FrameBuffer->Unbind();

    render_adapter::Get().GetRenderer().SetClearColor(ClearColor);
    render_adapter::Get().GetRenderer().SetWireframe(IsWireframe);
}

void test_gui_layer::OnGuiRender()
{
    if (IsDockspace)
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

    ImGui::Begin("Rendering Setup");
    {
        ImGui::Text("Object Tranformation");
        ImGui::SliderFloat("Size", &ObjectSize, 0.5f, 5.0f);
        ImGui::DragFloat3("Position", (float *)&ObjectPosition, 0.2, -100, 100);
        ImGui::SliderFloat("ObjectRotationX", &ObjectRotationX, -360, 360);
        ImGui::SliderFloat("ObjectRotationY", &ObjectRotationY, -360, 360);
        ImGui::SliderFloat("ObjectRotationZ", &ObjectRotationZ, -360, 360);

        ImGui::Separator();
        ImGui::Text("Renderer configuration.");
        if (ImGui::ColorEdit4("Clear Color", (float *)&ClearColor))
            render_adapter::Get().GetRenderer().SetClearColor(ClearColor);
        if (ImGui::Checkbox("Wireframe Mode", &IsWireframe))
            render_adapter::Get().GetRenderer().SetWireframe(IsWireframe);
        ImGui::Checkbox("Use Dock space", &IsDockspace);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();

    ImGui::Begin("Viewport");

    // Resize frame buffer
    ImVec2 window_size = ImGui::GetContentRegionAvail();
    const frame_buffer_props &view_props = FrameBuffer->GetFrameBufferProps();
    if ((view_props.Width != window_size.x || view_props.Height != window_size.y) &&
        window_size.x > 0 && window_size.y > 0)
    {
        frame_buffer_props new_view_props = view_props;
        new_view_props.Width = window_size.x;
        new_view_props.Height = window_size.y;
        FrameBuffer->SetFrameBufferProps(new_view_props);

        render_adapter::Get().RenderResize(window_size.x, window_size.y);
    }

    // Render framebuffer
    static ImVec2 uv1(0, 0), uv2(1, 1);
    ImGui::Image((void *)FrameBuffer->GetColorAttachmentInnerId(), ImVec2(view_props.Width, view_props.Height), uv2, uv1);

    ImGui::End();
}
