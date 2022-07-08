/*****************************************************************//**
 * \file   sandbox.cpp
 * \brief  
 * 
 * \author Sabitov Kirill
 * \date   04 July 2022
 *********************************************************************/

#include <sculpto.h>
#include <core/application/entry_point.h>
using namespace scl;

class sandbox_layer: public layer
{
public:
    sandbox_layer() = default;
    ~sandbox_layer() = default;

    shared<material_phong> ForestLeavesMaterial {};
    shared<material_phong> SphereMaterial {};
    shared<material_phong> CrateMaterial {};
    shared<material> SingleColorMaterial {};
    shared<mesh> CubeMesh {};
    shared<mesh> PlaneMesh {};
    shared<mesh> SphereMesh {};
    shared<mesh> WhiteSphereMesh {};
    shared<mesh> ConeMesh {};

    point_light PointLight {};
    directional_light DireLight {};
    spot_light SpotLight {};
    renderer_camera RenderCamera { camera_projection_type::PERSPECTIVE };

    void OnInit() override
    {
        shared<shader_program> basic_lighting_shader = shader_program::Create({
            { shader_type::VERTEX, "assets/shaders/phong.vert.glsl", false },
            { shader_type::PIXEL,  "assets/shaders/phong.frag.glsl", false },
        }, "basic_lighting_shader");

        shared<shader_program> single_color_shader = shader_program::Create({
            { shader_type::VERTEX, "assets/shaders/single_color.vert.glsl", false },
            { shader_type::PIXEL,  "assets/shaders/single_color.frag.glsl", false },
        }, "single_color_shader");

        ForestLeavesMaterial = material_phong::Create(basic_lighting_shader, vec3 {1, 0.5, 0.3 }, vec3 {}, 64.0f);
        ForestLeavesMaterial->SetDiffuseMapTexture(texture_2d::Create("assets/images/forest_leaves/forest_leaves_02_diffuse_4k.jpg"));

        SphereMaterial = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 { 0.7, 0.2, 0.4 }, 128.0f);

        CrateMaterial = material_phong::Create(basic_lighting_shader, vec3 { 1, 0.5, 0.3 }, vec3 {}, 64.0f);
        CrateMaterial->SetDiffuseMapTexture(texture_2d::Create("assets/images/crate.png"));
        CrateMaterial->SetSpecularMapTexture(texture_2d::Create("assets/images/crate_specular.png"));

        SingleColorMaterial = material::Create(single_color_shader);

        CubeMesh = mesh::Create<topology::cube>(vec3(0), vec3(1));
        PlaneMesh = mesh::Create<topology::grid>(10, 10);
        SphereMesh = mesh::Create<topology::sphere>(vec3(0), 1, 20);

        topology::sphere sphere_topo = topology::sphere(vec3 { 0 }, 1, 20);
        sphere_topo.SetColor(vec4 { 1 });
        WhiteSphereMesh = mesh::Create(sphere_topo);

        topology::cone cone_topo(vec3(0, 1, 0), 0, vec3(0.5, 0, 0.5), 0.4, 20);
        cone_topo.SetColor(vec4 { 0.2, 0.65, 0.38, 1 });
        ConeMesh = mesh::Create<>(cone_topo);

        RenderCamera.SetPosition({ 7, 6, -5 });
        render_bridge::SetClearColor(vec4{ 0.11, 0.09, 0.10, 1 });
    }

    void OnUpdate(float DeltaTime) override
    {
        static float delta = 0;
        if (delta > 1)
        {
            SCL_INFO("FPS: {}", timer::GetFps());
            delta = 0;
        }
        delta += timer::GetDeltaTime();

        render_bridge::Clear();

        renderer::StartRenderPass(RenderCamera.GetViewProjection(), RenderCamera.GetDirection(), RenderCamera.GetPosition(), 0, 0);

        renderer::Submit(CubeMesh, CrateMaterial,              matr4::Translate(vec3 { -0.5 }) * matr4::RotateX(timer::GetTime() * 30) * matr4::RotateY(timer::GetTime() * 30) * matr4::Translate(vec3 { 1 }));
        renderer::Submit(PlaneMesh, ForestLeavesMaterial,      matr4::Scale(vec3 { 5 }) * matr4::Translate(vec3 { -25, 0, -25 }));
        renderer::Submit(SphereMesh, SphereMaterial,           matr4::Scale(vec3 { 0.5 }) * matr4::Translate(vec3 { -2, 2, -2 }));
        renderer::Submit(WhiteSphereMesh, SingleColorMaterial, matr4::Scale(vec3 { 0.1 }) * matr4::Translate(vec3 { 2 }));
        // renderer::Submit(ConeMesh, SingleColorMaterial,        vec3 { 1 }, vec3 {}, vec3 { -5, 5, -5 });

        renderer::SubmitPointLight(vec3 { 2 }, vec3 { 1 }, 1.0f, 0.001f, 0.017f);
        renderer::SubmitDirectionalLight(vec3 { 0.1, -1, 0.5 }, vec3 { 0.3 });
        // renderer::SubmitSpotLight(vec3 { -5, 5, -5 }, vec3 { 0.5, -1, 0.5 }, vec3 { 0.2, 0.65, 0.38 }, cos(radians(degrees(15.0f))), cos(radians(degrees(30.0f))), cos(radians(degrees(15.0f))) - cos(radians(degrees(30.0f))));

        renderer::EndRenderPass();
    }

    void OnGuiRender() override
    {

    }

    void OnEvent(event &Event) override
    {
        event_dispatcher dispatcher(Event);
        dispatcher.Dispatch<window_resize_event>([&](window_resize_event &Event)
        {
            render_bridge::ResizeContext(Event.GetWidth(), Event.GetHeight());
            RenderCamera.Resize(Event.GetWidth(), Event.GetHeight());
            return false;
        });
        dispatcher.Dispatch<mouse_move_event>([&](mouse_move_event &Event)
        {
            if (Event.GetLButton())
            {
                RenderCamera.Rotate(vec3(0, 1, 0),
                                    input_system::GetMousePosDeltaX() *
                                    timer::GetDeltaTime() * 30);
                RenderCamera.Rotate(RenderCamera.GetRightDirection(),
                                    input_system::GetMousePosDeltaY() *
                                    timer::GetDeltaTime() * 30);
            }
            if (Event.GetRButton())
            {
                RenderCamera.Move((RenderCamera.GetRightDirection()  * -1) *
                                  input_system::GetMousePosDeltaX() *
                                  timer::GetDeltaTime() * 5);
                RenderCamera.Move(RenderCamera.GetUpDirection() *
                                  input_system::GetMousePosDeltaY() *
                                  timer::GetDeltaTime() * 5);
            }
            return true;
        });
        dispatcher.Dispatch<mouse_wheel_event>([&](mouse_wheel_event &Event)
        {
            if (input_system::GetMousePosDeltaZ() != 0)
            {
                RenderCamera.Move(RenderCamera.GetDirection() *
                                  input_system::GetMousePosDeltaZ() *
                                  timer::GetDeltaTime() * 5);
            }
            return true;
        });
        dispatcher.Dispatch<keyboard_event>([&](keyboard_event &Event)
        {
            if (Event.GetKeyCode() == keycode::D)
                RenderCamera.Move(RenderCamera.GetRightDirection() *
                                  timer::GetDeltaTime() * 55);
            if (Event.GetKeyCode() == keycode::A)
                RenderCamera.Move(RenderCamera.GetRightDirection() *
                                 -timer::GetDeltaTime() * 55);
            if (Event.GetKeyCode() == keycode::W)
                RenderCamera.Move(RenderCamera.GetDirection() *
                                  timer::GetDeltaTime() * 55);
            if (Event.GetKeyCode() == keycode::S)
                RenderCamera.Move(RenderCamera.GetDirection() *
                                 -timer::GetDeltaTime() * 55);
            return false;
        });
    }
};

class sandbox_app: public application
{
public:
    sandbox_app() : application("Sandbox")
    {
        PushOverlay(new sandbox_layer());
    }
    ~sandbox_app() override {}
    void OnInit() override {}
    void OnClose() override {}
};

scl::application *scl::CreateApplication()
{
    return new sandbox_app();
}