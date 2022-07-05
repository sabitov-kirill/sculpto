/*****************************************************************//**
 * \file   main.cpp
 * \brief  Sculpto sandbox program entry point.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include <imgui/imgui.h>

#include "editor_layer.h"

// AIMS FOR TODAY:
//   - diffrent types of light casters                             (+)
//   - api for easy passing structures to shader (glsl SSBOs)      ( )
//   - multiple lights in scene via components                     ( )

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
    auto basic_lighting_shader = shader_program::Create({
            { shader_type::VERTEX, "assets/shaders/phong.vert.glsl", false },
            { shader_type::PIXEL,  "assets/shaders/phong.frag.glsl", false },
    }, "basic_lighting_shader");
    auto single_color_shader = shader_program::Create({
        { shader_type::VERTEX, "assets/shaders/single_color.vert.glsl", false },
        { shader_type::PIXEL,  "assets/shaders/single_color.frag.glsl", false },
    }, "single_color_shader");

    auto sphere_material = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 { 0.7, 0.2, 0.4 }, 128.0f);

    auto forest_leaves_material = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 {}, 64.0f);
    forest_leaves_material->SetDiffuseMapTexture(texture_2d::Create("assets/images/forest_leaves/forest_leaves_02_diffuse_4k.jpg"));

    auto crate_material = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 {}, 64.0f);
    crate_material->SetDiffuseMapTexture(texture_2d::Create("assets/images/crate.png"));
    crate_material->SetSpecularMapTexture(texture_2d::Create("assets/images/crate_specular.png"));

    auto single_color_material = material::Create(single_color_shader);

    auto cube_mesh = mesh::Create<topology::cube>(vec3(0), vec3(1));
    auto plane_mesh = mesh::Create<topology::grid>(10, 10);
    auto sphere_mesh = mesh::Create<topology::sphere>(vec3 { 0 }, 1, 20);

    topology::sphere sphere_topo = topology::sphere(vec3 { 0 }, 1, 20);
    sphere_topo.SetColor({ 1 });
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
    plane.AddComponent<mesh_renderer_component>(forest_leaves_material);
    plane.AddComponent<transform_component>(vec3 { 5 }, vec3 { 0 }, vec3 { -25, 0, -25 });

    auto sphere = EditorScene->CreateObject();
    sphere.AddComponent<mesh_component>(sphere_mesh);
    sphere.AddComponent<mesh_renderer_component>(sphere_material);
    sphere.AddComponent<transform_component>(vec3 { 0.5 }, vec3 { 0 }, vec3 { -2, 2, -2 });

    auto light_bulb = EditorScene->CreateObject();
    light_bulb.AddComponent<mesh_component>(light_bulb_mesh);
    light_bulb.AddComponent<mesh_renderer_component>(single_color_material);
    light_bulb.AddComponent<transform_component>(vec3 { 0.1 }, vec3 { 0 }, vec3 { 2, 4, 2 });
 // light_bulb.AddComponent<point_light_coponent(pos, att, ...);

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
