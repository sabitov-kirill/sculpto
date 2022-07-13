/*****************************************************************//**
 * \file   main.cpp
 * \brief  Sculpto sandbox program entry point.
 *
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#include <imgui/imgui.h>

#include "editor_layer.h"

class camera_behaviour: public scl::scene_object_behaviour
{
    void OnUpdate() override
    {
        auto &camera = GetComponent<scl::camera_component>().Camera;

        if (scl::input_system::GetKey(scl::keycode::LBUTTON))
        {
            camera.Rotate(scl::vec3(0, 1, 0),
                          scl::input_system::GetMousePosDeltaX() *
                          scl::timer::GetDeltaTime() * 10);
            camera.Rotate(camera.GetRightDirection(),
                          scl::input_system::GetMousePosDeltaY() *
                          scl::timer::GetDeltaTime() * 10);
        }
        if (scl::input_system::GetKey(scl::keycode::RBUTTON))
        {
            camera.Move(camera.GetRightDirection() *
                       -scl::input_system::GetMousePosDeltaX() *
                        scl::timer::GetDeltaTime() * 3);
            camera.Move(camera.GetUpDirection() *
                        scl::input_system::GetMousePosDeltaY() *
                        scl::timer::GetDeltaTime() * 3);
        }
        if (scl::input_system::GetMousePosDeltaZ() != 0)
        {
            camera.Move(camera.GetDirection() *
                        scl::input_system::GetMousePosDeltaZ() *
                        scl::timer::GetDeltaTime() * 5);
        }
    }
};

class cube_behaviour: public scl::scene_object_behaviour
{
    void OnUpdate() override
    {
        auto &transform = GetComponent<scl::transform_component>();
        transform.Transform = scl::matr4::Rotate(scl::vec3 { 0.5, 0.3, 0.7 }, scl::timer::GetTime() * 15);
    }
};

void scl::editor::editor_layer::OnInit()
{
    EditorScene = CreateShared<scene>();

    auto wall_material = material_phong::Create(vec3 {}, vec3 {}, 8.0f);
    wall_material->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/stone_wall/diff.jpg"));
    wall_material->SetNormalMapTexture(assets_manager::LoadTexture("assets/images/stone_wall/bump.jpg"));
    auto wall = EditorScene->CreateObject();
    wall.AddComponent<mesh_component>(mesh::Create(topology::plane(4, 2), wall_material));
    wall.AddComponent<transform_component>(vec3 { 2 }, vec3 { 0, 0, 90 }, vec3 { -3, 14, -2 });
    
    auto crate_material = material_phong::Create(vec3 {}, vec3 { 0 }, 8.0f);
    // crate_material->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/crate.png"));
    // crate_material->SetSpecularMapTexture(assets_manager::LoadTexture("assets/images/crate_specular.png"));
    crate_material->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/crate/diff.png"));
    crate_material->SetNormalMapTexture(assets_manager::LoadTexture("assets/images/crate/bump.jpg"));
    auto cube = EditorScene->CreateObject();
    cube.AddComponent<mesh_component>(mesh::Create(topology::cube(vec3(0), vec3(1)), crate_material));
    cube.AddComponent<transform_component>(vec3 { 1 }, vec3 { 0 }, vec3 { 1 });
    cube.AddComponent<native_script_component>().Bind<cube_behaviour>();
    
    auto floor_matrerial = material_phong::Create(vec3 { 1, 0.5, 0.3 }, vec3 { 0.6, 0.5, 0.55 }, 128.0f);
    floor_matrerial->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/wood.png"));
    auto plane = EditorScene->CreateObject();
    plane.AddComponent<mesh_component>(mesh::Create(topology::plane(10, 10), floor_matrerial));
    plane.AddComponent<transform_component>(vec3 { 5 }, vec3 { 0 }, vec3 { -25, 0, -25 });
    
    auto sphere = EditorScene->CreateObject();
    sphere.AddComponent<mesh_component>(mesh::Create(topology::sphere(vec3 { 0 }, 1, 20),
    material_phong::Create(vec3 { 1, 0.5, 0.3 }, vec3 { 0.7, 0.9, 0.9 }, 128.0f)));
    sphere.AddComponent<transform_component>(vec3 { 0.5 }, vec3 { 0 }, vec3 { -2, 2, -2 });
    
    vec3 col = vec3 { 1 };
    topology::sphere sphere_topo = topology::sphere(vec3 { 0 }, 1, 20);
    auto light_bulb = EditorScene->CreateObject("Point Light");
    light_bulb.AddComponent<mesh_component>(mesh::Create(sphere_topo, material_single_color::Create(col)));
    light_bulb.AddComponent<transform_component>(vec3 { 0.1 }, vec3 { 0 }, vec3 { 2, 4, 2 });
    light_bulb.AddComponent<point_light_component>(col, 1.0f, 0.022f, 0.0019f);
    
    vec3 A = vec3(0, 1, 0), B = vec3(-2, 0, 1);
    col = vec3 { 0.55, 1, 0.45 };
    auto cone = EditorScene->CreateObject();
    cone.AddComponent<mesh_component>(mesh::Create(topology::cone(A, 0, B, 0.4, 20), material_single_color::Create(col)));
    cone.AddComponent<transform_component>(vec3 { 1 }, vec3 {}, vec3 { 5, 5, -5 });
    cone.AddComponent<spot_light_component>(col, (B - A).Normalized(), 15.0f, 30.0f);

    auto projector = EditorScene->CreateObject("Directional Light");
    projector.AddComponent<directional_light_component>(vec3 { 0, -1, -0.2 }, vec3 { 3 }, true, 40.0f, 100.0f, 4096, 4096);

     auto model = EditorScene->CreateObject("Model, loaded from file");
     model.AddComponent<mesh_component>(assets_manager::LoadMeshes("assets/models/sponza/sponza.gltf"));
     model.AddComponent<transform_component>(vec3 { 0.02 }, vec3 { 0, 0, 0 }, vec3 { -2, 4, 4 });
     // model.AddComponent<native_script_component>().Bind<cube_behaviour>();

    camera rend_camera { camera_projection_type::PERSPECTIVE, camera_effects { true, 0.7, true, 8 } };
    rend_camera.SetPosition({ 4, 5, -5 });
    auto camera = EditorScene->CreateObject("MainCamera");
    camera.AddComponent<camera_component>(rend_camera);
    camera.AddComponent<native_script_component>().Bind<camera_behaviour>();
    EditorScene->SetMainCamera("MainCamera");

    // MainViewportWindow = CreateShared<viewport_window>(EditorScene->GetMainCamera().MainFrameBuffer, true, EditorScene.get());
    MainViewportWindow = CreateShared<viewport_window>(EditorScene->GetMainCamera().Camera.GetMainFrameBuffer(), true, EditorScene.get());
    ConfigWindow = CreateShared<application_config_window>();
    ProfillerWindow = CreateShared<profiller_window>();
    SceneConfigWindow = CreateShared<scene_config_window>(EditorScene.get());
    SceneObjectConfigWindow = CreateShared<scene_object_config_window>();
    SceneHierarchyWindow = CreateShared<scene_hierarchy_window>(EditorScene.get(), [&](scene_object SelectedObject)
    {
        SceneObjectConfigWindow->SetConfiguringObject(SelectedObject);
    });

    // rend_camera.SetRenderToSwapChain(true);
    // application::Get().GuiEnabled = false;
}

void scl::editor::editor_layer::OnUpdate(float DeltaTime)
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

void scl::editor::editor_layer::OnGuiRender()
{
    ConfigWindow->Draw();
    MainViewportWindow->Draw();
    ProfillerWindow->Draw();
    SceneConfigWindow->Draw();
    SceneHierarchyWindow->Draw();
    SceneObjectConfigWindow->Draw();

    ImGui::ShowDemoWindow();
}
