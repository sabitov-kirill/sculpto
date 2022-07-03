/*****************************************************************//**
 * \file   main.cpp
 * \brief  Sculpto sandbox program entry point.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include <entt.hpp>
#include <imgui/imgui.h>

#include "editor_layer.h"

class camera_behaviour: public scene_object_behaviour
{
    void OnCreate() override
    {
    }

    void OnDestroy() override
    {
    }

    void OnUpdate() override
    {
        auto &renderer_camera = GetComponent<camera_controller_component>().Camera;

        if (input_system::GetKey(keycode::LBUTTON))
        {
            renderer_camera.Rotate(vec3(0, 1, 0),
                                   -input_system::GetMousePosDeltaX() *
                                   timer::GetDeltaTime() * 100);
            renderer_camera.Rotate(renderer_camera.GetRightDirection(),
                                   input_system::GetMousePosDeltaY() *
                                   timer::GetDeltaTime() * 100);
        }
        if (input_system::GetKey(keycode::RBUTTON))
        {
            renderer_camera.Move(renderer_camera.GetRightDirection() *
                                 input_system::GetMousePosDeltaX() *
                                 timer::GetDeltaTime() * 10);
            renderer_camera.Move(renderer_camera.GetUpDirection() *
                                 input_system::GetMousePosDeltaY() *
                                 timer::GetDeltaTime() * 10);
        }
        if (input_system::GetMousePosDeltaZ() != 0)
        {
            renderer_camera.Move(renderer_camera.GetDirection() *
                                 input_system::GetMousePosDeltaZ() *
                                 timer::GetDeltaTime() * 10);
        }
    }
};

void editor::editor_layer::OnInit()
{
    /*
     * Creating render primitves
     */
    shared<shader_program> sandbox_shader = shader_program::Create({
        { shader_type::VERTEX, "assets/shaders/Test.vert.glsl", false },
        { shader_type::PIXEL,  "assets/shaders/Test.frag.glsl", false },
    }, "Test rendering shader");
    shared<material> stone_material = material::Create(sandbox_shader, nullptr, 0, "assets/images/stone.jpg");
    shared<material> zerotwo_material = material::Create(sandbox_shader, nullptr, 0, "assets/images/02.jpg");
    shared<material> chat_material = material::Create(sandbox_shader, nullptr, 0, "assets/images/chat.jpg");

    topology::cube cube_topo(vec3(0), vec3(1));
    shared<mesh> cube_mesh = mesh::Create(cube_topo);

    topology::grid plane_topo(1, 1);
    shared<mesh> plane_mesh = mesh::Create(plane_topo);

    renderer_camera rend_camera { camera_projection_type::PERSPECTIVE };
    rend_camera.SetPosition({ 4, 5, -5 });

    render_bridge::SetClearColor(ClearColor);
    render_bridge::SetWireframeMode(IsWireframe);

    /*
     * Creating scene objects
     */
    EditorScene = CreateShared<scene>();

    auto cube = EditorScene->CreateObject();
    cube.AddComponent<mesh_component>(cube_mesh);
    cube.AddComponent<mesh_renderer_component>(zerotwo_material);
    cube.AddComponent<transform_component>(vec3(1), vec3(0), vec3(1));

    auto plane = EditorScene->CreateObject();
    plane.AddComponent<mesh_component>(plane_mesh);
    plane.AddComponent<mesh_renderer_component>(chat_material);
    plane.AddComponent<transform_component>(vec3(10), vec3(0), vec3(-5, 0, -5));

    auto camera = EditorScene->CreateObject("MainCamera");
    camera.AddComponent<camera_controller_component>(rend_camera);
    camera.AddComponent<native_script_component>().Bind<camera_behaviour>();

    EditorScene->SetMainCamera("MainCamera");
    // EditorScene->SetRenderToSwapChainBuffer(true);
    // application::Get().SetGuiEnabled(false);
}

void editor::editor_layer::OnUpdate(float DeltaTime)
{
#if 0
    static float delta = 0;
    if (delta > 1)
    {
        SCL_INFO("FPS: {}", timer::GetFps());
        delta = 0;
    }
    delta += timer::GetDeltaTime();
#endif

    EditorScene->OnUpdate(DeltaTime);
}

void editor::editor_layer::OnGuiRender()
{
    if (IsDockspace) DrawDockSpace();
    DrawViewportWindow();
    DrawRendererConfigWindow();
}

void editor::editor_layer::OnEvent(event &Event)
{
    if (Event.GetType() != SCL_STRING_ID("window_resize_event"))
        EditorScene->OnEvent(Event);
}
