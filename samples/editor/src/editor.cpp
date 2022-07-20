/*****************************************************************//**
 * \file   editor.h
 * \brief  Sculpto editor application implementation module.
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include <sculpto.h>
#include <core/application/entry_point.h>
using namespace scl;

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

class editor_app: public application
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
    editor_app() : application("Editor") {}
    ~editor_app() override {}

    void OnInit() override
    {
        EditorScene = CreateShared<scene>();

#if 1
        auto wall_material = material_phong::Create(vec3 {}, vec3 {}, 8.0f);
        wall_material->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/stone_wall/diff.jpg"));
        wall_material->SetNormalMapTexture(assets_manager::LoadTexture("assets/images/stone_wall/bump.jpg"));
        auto wall = EditorScene->CreateObject("Wall");
        wall.AddComponent<mesh_component>(mesh::Create(topology::plane(10, 10), wall_material));
        wall.AddComponent<transform_component>(vec3 { 5 }, vec3 { 0 }, vec3 { -25, 0, -25 });

        auto crate_material = material_phong::Create(vec3 {}, vec3 { 0 }, 8.0f);
        // crate_material->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/crate.png"));
        // crate_material->SetSpecularMapTexture(assets_manager::LoadTexture("assets/images/crate_specular.png"));
        crate_material->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/crate/diff.png"));
        crate_material->SetNormalMapTexture(assets_manager::LoadTexture("assets/images/crate/bump.jpg"));
        auto cube = EditorScene->CreateObject("Cube");
        cube.AddComponent<mesh_component>(mesh::Create(topology::cube(vec3(0), vec3(1)), crate_material));
        cube.AddComponent<transform_component>(vec3 { 1 }, vec3 { 0 }, vec3 { 1 });
        cube.AddComponent<native_script_component>().Bind<cube_behaviour>();

        auto floor_matrerial = material_phong::Create(vec3 { 1, 0.5, 0.3 }, vec3 { 0.6, 0.5, 0.55 }, 128.0f);
        floor_matrerial->SetDiffuseMapTexture(assets_manager::LoadTexture("assets/images/wood.png"));
        auto plane = EditorScene->CreateObject("Floor");
        plane.AddComponent<mesh_component>(mesh::Create(topology::plane(10, 10), floor_matrerial));
        plane.AddComponent<transform_component>(vec3 { 5 }, vec3 { 0 }, vec3 { -25, 0, -25 });
        
        auto sphere = EditorScene->CreateObject("Sphere");
        sphere.AddComponent<mesh_component>(mesh::Create(topology::sphere(vec3 { 0 }, 1, 20), material_phong::Create(vec3 { 1, 0.5, 0.3 }, vec3 { 0.7, 0.9, 0.9 }, 128.0f)));
        sphere.AddComponent<transform_component>(vec3 { 0.5 }, vec3 { 0 }, vec3 { -2, 2, -2 });

        vec3 col = vec3 { 1 };
        topology::sphere sphere_topo = topology::sphere(vec3 { 0 }, 1, 20);
        auto light_bulb = EditorScene->CreateObject("Point Light");
        light_bulb.AddComponent<mesh_component>(mesh::Create(sphere_topo, material_single_color::Create(col)));
        light_bulb.AddComponent<transform_component>(vec3 { 0.1 }, vec3 { 0 }, vec3 { 2, 4, 2 });
        light_bulb.AddComponent<point_light_component>(col, 1.0f, 0.022f, 0.0019f);

        vec3 A = vec3(0, 1, 0), B = vec3(0);
        col = vec3 { 0.55, 1, 0.45 };
        auto cone = EditorScene->CreateObject("Spot Light");
        cone.AddComponent<mesh_component>(mesh::Create(topology::cone(A, 0, B, 0.4, 20), material_single_color::Create(col)));
        cone.AddComponent<transform_component>(vec3 { 1 }, vec3 {}, vec3 { 5, 5, -5 });
        cone.AddComponent<spot_light_component>(col, 15.0f, 30.0f);

        auto projector = EditorScene->CreateObject("Directional Light");
        projector.AddComponent<transform_component>(vec3 { 1 }, vec3 { 5, 0, 30 }, vec3 { 5, 50, -5 });
        projector.AddComponent<directional_light_component>(vec3 { 1 }, true, 40.0f, 100.0f, 4096, 4096);

        // auto model = EditorScene->CreateObject("Model, loaded from file");
        // model.AddComponent<mesh_component>(assets_manager::LoadMeshes("assets/models/sponza/sponza.gltf"));
        // model.AddComponent<transform_component>(vec3 { 0.02 }, vec3 { 0, 0, 0 }, vec3 { -2, 4, 4 });
        // // model.AddComponent<native_script_component>().Bind<cube_behaviour>();

        camera render_camera { camera_projection_type::PERSPECTIVE, camera_effects { true, 0.7, true, 8 } };
        render_camera.SetPosition({ 4, 5, -5 });
        auto camera = EditorScene->CreateObject("Main Camera");
        camera.AddComponent<camera_component>(render_camera, true);
        camera.AddComponent<native_script_component>().Bind<camera_behaviour>();
#endif

        //scene_serializer::Deserialize(EditorScene, "assets/scenes/test.scl");
        // auto camera = EditorScene->CreaetOrGetObject("Main Camera");
        // camera.AddComponent<native_script_component>().Bind<camera_behaviour>();

        MainViewportWindow = CreateShared<viewport_window>(camera.GetComponent<camera_component>().Camera.GetMainFrameBuffer());
        ConfigWindow = CreateShared<application_config_window>();
        ProfillerWindow = CreateShared<profiller_window>();
        SceneConfigWindow = CreateShared<scene_config_window>(EditorScene.get());
        SceneObjectConfigWindow = CreateShared<scene_object_config_window>();
        SceneHierarchyWindow = CreateShared<scene_hierarchy_window>(EditorScene.get(), [&](scene_object SelectedObject)
        {
            SceneObjectConfigWindow->SetConfiguringObject(SelectedObject);
        });

        application::Get().GuiEnabled = true;
    }

    void OnClose() override
    {
        //scene_serializer::Serialize(EditorScene, "assets/scenes/test.scl");
    }

    void OnUpdate(float DeltaTime) override
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

        EditorScene->Update();
        EditorScene->Render();
    }

    void OnGuiUpdate()
    {
        ConfigWindow->Draw();
        MainViewportWindow->Draw();
        ProfillerWindow->Draw();
        SceneConfigWindow->Draw();
        SceneHierarchyWindow->Draw();
        SceneObjectConfigWindow->Draw();

        ImGui::ShowDemoWindow();
    }
};

scl::application *scl::CreateApplication()
{
    return new editor_app();
}