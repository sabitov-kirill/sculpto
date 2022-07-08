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

    auto sphere_material = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 { 0.7, 0.9, 0.9 }, 128.0f);

    auto floor_matrerial = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 { 0.6, 0.5, 0.55 }, 128.0f);
 // floor_matrerial->SetDiffuseMapTexture(texture_2d::Create("assets/images/forest_leaves/forest_leaves_02_diffuse_4k.jpg"));
    floor_matrerial->SetDiffuseMapTexture(texture_2d::Create("assets/images/wood.png"));

    auto crate_material = material_phong::Create(basic_lighting_shader, vec3 {}, vec3 {}, 8.0f);
    crate_material->SetDiffuseMapTexture(texture_2d::Create("assets/images/crate.png"));
    crate_material->SetSpecularMapTexture(texture_2d::Create("assets/images/crate_specular.png"));

    auto single_color_material = material::Create(single_color_shader);

    auto cube_mesh = mesh::Create<topology::cube>(vec3(0), vec3(1));
    auto plane_mesh = mesh::Create<topology::grid>(10, 10);
    auto sphere_mesh = mesh::Create<topology::sphere>(vec3 { 0 }, 1, 20);

    topology::cone cone_topo(vec3(0, 1, 0), 0, vec3(0.5, 0, 0.5), 0.4, 20);
    cone_topo.SetColor(vec4 { 0.2, 0.65, 0.38, 1 });
    auto cone_mesh = mesh::Create(cone_topo);

    topology::sphere sphere_topo = topology::sphere(vec3 { 0 }, 1, 20);
    sphere_topo.SetColor(vec4 { 1 });
    auto light_bulb_mesh = mesh::Create(sphere_topo);

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
    cube.AddComponent<mesh_renderer_component>(crate_material);
    cube.AddComponent<transform_component>(vec3 { 1 }, vec3 { 0 }, vec3 { 1 });
    cube.AddComponent<native_script_component>().Bind<cube_behaviour>();

    auto plane = EditorScene->CreateObject();
    plane.AddComponent<mesh_component>(plane_mesh);
    plane.AddComponent<mesh_renderer_component>(floor_matrerial);
    plane.AddComponent<transform_component>(vec3 { 5 }, vec3 { 0 }, vec3 { -25, 0, -25 });

    auto sphere = EditorScene->CreateObject();
    sphere.AddComponent<mesh_component>(sphere_mesh);
    sphere.AddComponent<mesh_renderer_component>(sphere_material);
    sphere.AddComponent<transform_component>(vec3 { 0.5 }, vec3 { 0 }, vec3 { -2, 2, -2 });

    auto cone = EditorScene->CreateObject();
    cone.AddComponent<mesh_component>(cone_mesh);
    cone.AddComponent<mesh_renderer_component>(single_color_material);
    cone.AddComponent<transform_component>(vec3 { 1 }, vec3 {}, vec3 { -5, 5, -5 });
    cone.AddComponent<spot_light_component>(vec3 { 0.2, 0.65, 0.38 }, vec3 { 0.5, -1, 0.5 }, 15.0f, 30.0f);
    
    auto light_bulb = EditorScene->CreateObject();
    light_bulb.AddComponent<mesh_component>(light_bulb_mesh);
    light_bulb.AddComponent<mesh_renderer_component>(single_color_material);
    light_bulb.AddComponent<transform_component>(vec3 { 0.1 }, vec3 { 0 }, vec3 { 2, 1, 2 });
    light_bulb.AddComponent<point_light_component>(vec3 { 1 }, 1.0f, 0.001f, 0.017f);

    auto projector = EditorScene->CreateObject();
    projector.AddComponent<directional_light_component>(vec3 { -1, -1, 0.5 }, vec3 { 0.5, 0.5, 0.5 }, true, 10.0f, 10.0f);

    auto camera = EditorScene->CreateObject("MainCamera");
    camera.AddComponent<camera_controller_component>(rend_camera);
    camera.AddComponent<native_script_component>().Bind<camera_behaviour>();
    EditorScene->SetMainCamera("MainCamera");
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
