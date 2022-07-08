/*****************************************************************//**
 * \file   main.cpp
 * \brief  Sculpto sandbox program entry point.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include <imgui/imgui.h>

#include "editor_layer.h"

class camera_behaviour: public scene_object_behaviour
{
    void OnUpdate() override
    {
        auto &renderer_camera = GetComponent<camera_controller_component>().Camera;

        if (input_system::GetKey(keycode::LBUTTON))
        {
            renderer_camera.Rotate(vec3(0, 1, 0),
                                  -input_system::GetMousePosDeltaX() *
                                   timer::GetDeltaTime() * 25);
            renderer_camera.Rotate(renderer_camera.GetRightDirection(),
                                   input_system::GetMousePosDeltaY() *
                                   timer::GetDeltaTime() * 25);
        }
        if (input_system::GetKey(keycode::RBUTTON))
        {
            renderer_camera.Move(renderer_camera.GetRightDirection() *
                                 input_system::GetMousePosDeltaX() *
                                 timer::GetDeltaTime() * 5);
            renderer_camera.Move(renderer_camera.GetUpDirection() *
                                 input_system::GetMousePosDeltaY() *
                                 timer::GetDeltaTime() * 5);
        }
        if (input_system::GetMousePosDeltaZ() != 0)
        {
            renderer_camera.Move(renderer_camera.GetDirection() *
                                 input_system::GetMousePosDeltaZ() *
                                 timer::GetDeltaTime() * 10);
        }
    }
};

class cube_behaviour: public scene_object_behaviour
{
    void OnUpdate() override
    {
        auto &transform = GetComponent<transform_component>();
        transform.Transform =
            matr4::Translate(vec3 { -0.5 }) *
            matr4::RotateX(timer::GetTime() * 30) *
            matr4::RotateY(timer::GetTime() * 30) *
            matr4::Translate(vec3 { 1 });
    }
};

void editor::editor_layer::OnInit()
{
    /*
     * Creating render primitves
     */
    auto basic_lighting_shader = assets_manager::LoadShader("assets/shaders/phong.glsl");
    auto single_color_shader = assets_manager::LoadShader("assets/shaders/single_color.glsl");

    /*
     * Creating scene objects
     */
    EditorScene = CreateShared<scene>();

    auto wall_material = material_phong::Create(basic_lighting_shader, vec3 {}, vec3 {}, 8.0f);
    wall_material->SetDiffuseMapTexture(texture_2d::Create("assets/images/stone_wall/diff.jpg"));
    wall_material->SetNormalMapTexture(texture_2d::Create("assets/images/stone_wall/bump.jpg"));
    auto wall = EditorScene->CreateObject();
    wall.AddComponent<mesh_component>(mesh::Create<topology::plane>(4, 2));
    wall.AddComponent<mesh_renderer_component>(wall_material);
    wall.AddComponent<transform_component>(vec3 { 2 }, vec3 { 0, 0, 90 }, vec3 { -3, 4, -2 });

    auto crate_material = material_phong::Create(basic_lighting_shader, vec3 {}, vec3 { 0 }, 8.0f);
    crate_material->SetDiffuseMapTexture(texture_2d::Create("assets/images/crate.png"));
    crate_material->SetSpecularMapTexture(texture_2d::Create("assets/images/crate_specular.png"));
    //crate_material->SetDiffuseMapTexture(texture_2d::Create("assets/images/crate/diff.png"));
    //crate_material->SetNormalMapTexture(texture_2d::Create("assets/images/crate/bump.jpg"));
    auto cube = EditorScene->CreateObject();
    cube.AddComponent<mesh_component>(mesh::Create<topology::cube>(vec3(0), vec3(1)));
    cube.AddComponent<mesh_renderer_component>(crate_material);
    cube.AddComponent<transform_component>(vec3 { 1 }, vec3 { 0 }, vec3 { 1 });
    cube.AddComponent<native_script_component>().Bind<cube_behaviour>();

    auto floor_matrerial = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 { 0.6, 0.5, 0.55 }, 128.0f);
    floor_matrerial->SetDiffuseMapTexture(texture_2d::Create("assets/images/wood.png"));
    auto plane = EditorScene->CreateObject();
    plane.AddComponent<mesh_component>(mesh::Create<topology::plane>(10, 10));
    plane.AddComponent<mesh_renderer_component>(floor_matrerial);
    plane.AddComponent<transform_component>(vec3 { 5 }, vec3 { 0 }, vec3 { -25, 0, -25 });

    auto sphere = EditorScene->CreateObject();
    sphere.AddComponent<mesh_component>(mesh::Create<topology::sphere>(vec3 { 0 }, 1, 20));
    sphere.AddComponent<mesh_renderer_component>(material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 { 0.7, 0.9, 0.9 }, 128.0f));
    sphere.AddComponent<transform_component>(vec3 { 0.5 }, vec3 { 0 }, vec3 { -2, 2, -2 });

    vec3 col = vec3 { 1 };
    topology::sphere sphere_topo = topology::sphere(vec3 { 0 }, 1, 20);
    auto light_bulb = EditorScene->CreateObject();
    light_bulb.AddComponent<mesh_component>(mesh::Create(sphere_topo));
    light_bulb.AddComponent<mesh_renderer_component>(material_single_color::Create(single_color_shader, col));
    light_bulb.AddComponent<transform_component>(vec3 { 0.1 }, vec3 { 0 }, vec3 { 2, 4, 2 });
    light_bulb.AddComponent<point_light_component>(col, 1.0, 0.022, 0.0019);

    auto projector = EditorScene->CreateObject();
    projector.AddComponent<directional_light_component>(vec3 { -1, -1, 0 }, vec3 { 0.2, 0.2, 0.2 }, true, 10.0f, 10.0f);

    vec3 A = vec3(0, 1, 0), B = vec3(-2, 0, 1);
    col = vec3 { 0.55, 0.85, 0.45 };
    topology::cone cone_topo(A, 0, B, 0.4, 20);
    auto cone_mesh = mesh::Create(cone_topo);
    auto cone = EditorScene->CreateObject();
    cone.AddComponent<mesh_component>(cone_mesh);
    cone.AddComponent<mesh_renderer_component>(material_single_color::Create(single_color_shader, col));
    cone.AddComponent<transform_component>(vec3 { 1 }, vec3 {}, vec3 { 5, 5, -5 });
    cone.AddComponent<spot_light_component>(col, (B - A).Normalized(), 15.0f, 30.0f);

    renderer_camera rend_camera { camera_projection_type::PERSPECTIVE };
    rend_camera.SetPosition({ 4, 5, -5 });
    auto camera = EditorScene->CreateObject("MainCamera");
    camera.AddComponent<camera_controller_component>(rend_camera);
    camera.AddComponent<native_script_component>().Bind<camera_behaviour>();
    EditorScene->SetMainCamera("MainCamera");

    render_bridge::SetClearColor(ClearColor);
    render_bridge::SetWireframeMode(IsWireframe);
    // EditorScene->SetRenderToSwapChainBuffer(true);
    // application::Get().SetGuiEnabled(false);
}

void editor::editor_layer::OnUpdate(float DeltaTime)
{
#if 1
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
