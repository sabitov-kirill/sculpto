/*****************************************************************//**
 * \file   raytracing.cpp
 * \brief  GPU raytracing based on sculpto library main module.
 * 
 * \author Sabitov Kirill
 * \date   15 July 2022
 *********************************************************************/

#include <sculpto.h>
#include <core/application/entry_point.h>
using namespace scl;

class raytracing_app: public application
{
    struct surface
    {
        vec3  Emmitance {};
        float Roughness {};
        vec3  Reflectance {};
        float Opacity {};

        surface() = default;
        surface(const surface &Other) = default;
        surface(vec3 Emmitance, float Roughness, vec3  Reflectance, float Opacity) :
            Emmitance(Emmitance), Roughness(Roughness), Reflectance(Reflectance), Opacity(Opacity) {}
    };

    struct sphere
    {
        vec3  Center {};
        float  Radius {};
        surface  Surface {};

        sphere() = default;
        sphere(const sphere &Other) = default;
        sphere(vec3 Center, float Radius, surface Surface) :
            Center(Center), Radius(Radius), Surface(Surface) {}

    };

    struct plane
    {
        vec3 Normal {};
        float Dist {};
        surface Surface {};

        plane() = default;
        plane(const plane &Other) = default;
        plane(float Dist, vec3 Normal, surface Surface) :
            Dist(Dist), Normal(Normal), Surface(Surface) {}
    };

    struct box
    {
        vec3 Size {};
        float dummy1 {};
        matr4_data Rotation {};
        vec3 Position {};
        float dummy2 {};
        surface Surface {};

        box() = default;
        box(const box &Other) = default;
        box(vec3 Size, matr4 Rotation, vec3 Position, surface Surface) :
            Size(Size), Rotation(Rotation), Position(Position), Surface(Surface) {}
    };

#define MAX_SPHERE_COUNT 100
#define MAX_BOX_COUNT    100
#define MAX_PLANES_COUNT 100

    struct scene_objects
    {
        sphere Spheres[MAX_SPHERE_COUNT] {};
        box    Boxes[MAX_BOX_COUNT] {};
        plane  Planes[MAX_PLANES_COUNT] {};
        u32    SpheresCount {};
        u32    BoxesCount {};
        u32    PlanesCount {};
    };

    struct scene_data
    {
        vec3   CameraPosition;
        float  CameraProjectionDistance;
        vec3   CameraDirection;
        float  ViewportWidth;
        vec3   CameraRightDirection;
        float  ViewportHeight;
        vec3   CameraUpDirection;
        float  ViewportProjectionWidth;
        float  ViewportProjectionHeight;
        float  Time;
        u32    Samples;
    };

private:
    camera Camera { camera_projection_type::PERSPECTIVE };
    vec3 PrevFrameCameraPosition {};
    vec3 PrevFrameCameraDirection {};
    vec3 PrevFrameCameraUpDirection {};
    vec3 PrevFrameCameraRightDirection {};
    u32 AccumulatedFramesCount;

    shared<viewport_window> MainViewportWindow {};
    shared<shader_program>  RayTracingShader {};
    shared<shader_program>  RayTracingFrameDrawShader {};
    shared<constant_buffer> SceneDataBuffer {};
    shared<constant_buffer> SceneObjectsBuffer {};
    scene_data SceneData {};
    scene_objects SceneObjects {};
    bool IsImageStatic { true };

    void UpdateSceneData()
    {
        SceneData.CameraDirection          = Camera.GetDirection();
        SceneData.CameraPosition           = Camera.GetPosition();
        SceneData.CameraProjectionDistance = Camera.GetProjectionDistance();
        SceneData.CameraRightDirection     = Camera.GetRightDirection();
        SceneData.CameraUpDirection        = Camera.GetUpDirection();
        SceneData.ViewportWidth            = Camera.GetViewportWidth();
        SceneData.ViewportHeight           = Camera.GetViewportHeight();
        SceneData.ViewportProjectionWidth  = Camera.GetViewportProjectionWidth();
        SceneData.ViewportProjectionHeight = Camera.GetViewportProjectionHeight();
        SceneData.Time                     = timer::GetTime();
        SceneData.Samples                  = IsImageStatic ? 16 : 4;
        SceneDataBuffer->Update(&SceneData, sizeof(scene_data));
    }

public:
    raytracing_app() : application("PathTracer") {}
    ~raytracing_app() override {}

    void OnInit() override
    {
        application::Get().GuiEnabled = true;

        RayTracingShader = assets_manager::LoadShader("assets/shaders/raytracing_main.glsl");
        RayTracingFrameDrawShader = assets_manager::LoadShader("assets/shaders/raytracing_frame_draw.glsl");
        SceneDataBuffer = constant_buffer::Create(sizeof(scene_data));
        SceneObjectsBuffer = constant_buffer::Create(sizeof(scene_objects));
        MainViewportWindow = CreateShared<viewport_window>(Camera.GetMainFrameBuffer());

        Camera.SetRenderToSwapChain(false);
        Camera.SetView(vec3 { 0, 0.001, 17 }, vec3 { 0, 0, 0 }, vec3 { 0, 1, 0 });
        Camera.Resize(application::GetWindow().GetWindowData().Width, application::GetWindow().GetWindowData().Height);
        event_dispatcher::AddEventListner<viewport_resize_event>([&](viewport_resize_event &Event)
        {
            if (Event.GetViewportId() == 30)
                Camera.Resize(Event.GetWidth(), Event.GetHeight());
            return false;
        });

        event_dispatcher::AddEventListner<keyboard_event>([&](keyboard_event &Event)
        {
            if (Event.GetKeyCode() == keycode::R && Event.GetClicked())
                assets_manager::UpdateShader(RayTracingShader);
            return false;
        });

        event_dispatcher::AddEventListner<mouse_move_event>([&](mouse_move_event &Event)
        {
            if (scl::input_system::GetKey(scl::keycode::LBUTTON))
            {
                Camera.Rotate(scl::vec3(0, 1, 0),
                              scl::input_system::GetMousePosDeltaX() *
                              scl::timer::GetDeltaTime() * 2);
                Camera.Rotate(Camera.GetRightDirection(),
                              scl::input_system::GetMousePosDeltaY() *
                              scl::timer::GetDeltaTime() * 2);
            }
            if (scl::input_system::GetKey(scl::keycode::RBUTTON))
            {
                Camera.Move(Camera.GetRightDirection() *
                            -scl::input_system::GetMousePosDeltaX() *
                            scl::timer::GetDeltaTime() * 2);
                Camera.Move(Camera.GetUpDirection() *
                            scl::input_system::GetMousePosDeltaY() *
                            scl::timer::GetDeltaTime() * 2);
            }
            return true;
        });

        event_dispatcher::AddEventListner<mouse_wheel_event>([&](mouse_wheel_event &Event)
        {
            if (scl::input_system::GetMousePosDeltaZ() != 0)
            {
                Camera.Move(Camera.GetDirection() *
                            scl::input_system::GetMousePosDeltaZ() *
                            scl::timer::GetDeltaTime() / 5);
            }
            return true;
        });

        InitCernellBoxScene();
        // InitializeSpheresFieldScene();
    }

    void OnClose() override {}

    void OnUpdate(float DeltaTime) override
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

        IsImageStatic = PrevFrameCameraPosition == Camera.GetPosition() &&
                        PrevFrameCameraDirection == Camera.GetDirection() &&
                        PrevFrameCameraUpDirection == Camera.GetUpDirection() &&
                        PrevFrameCameraRightDirection == Camera.GetRightDirection();
        UpdateSceneData();

        // Render frame to hdr frame buffer (with static samples count).
        if (!IsImageStatic) Camera.GetHDRFrameBuffer()->Clear();
        Camera.GetHDRFrameBuffer()->Bind();

        SceneDataBuffer->Bind(0);
        SceneObjectsBuffer->Bind(1);
        RayTracingShader->Bind();
        Camera.GetHDRFrameBuffer()->GetColorAttachment()->Bind(0);
        renderer::DrawFullscreenQuad();

        Camera.GetHDRFrameBuffer()->Unbind();

        PrevFrameCameraPosition = Camera.GetPosition();
        PrevFrameCameraDirection = Camera.GetDirection();
        PrevFrameCameraUpDirection = Camera.GetUpDirection();
        PrevFrameCameraRightDirection = Camera.GetRightDirection();

        // Draw accumulated frames
        Camera.GetMainFrameBuffer()->Clear();
        Camera.GetMainFrameBuffer()->Bind();

        AccumulatedFramesCount = IsImageStatic ? AccumulatedFramesCount + 1 : 1;
        RayTracingFrameDrawShader->Bind();
        RayTracingFrameDrawShader->SetUInt("u_AccumulatedFramesCount", AccumulatedFramesCount);
        Camera.GetHDRFrameBuffer()->GetColorAttachment()->Bind(0);
        renderer::DrawFullscreenQuad();

        Camera.GetMainFrameBuffer()->Unbind();
    }

    void OnGuiUpdate() override
    {
        MainViewportWindow->Draw();
    }

    void InitCernellBoxScene()
    {
        SceneObjects.SpheresCount = 3;
        SceneObjects.BoxesCount = 9;

        SceneObjects.Spheres[0] = sphere(vec3(2.5, -1.0, -1.0), 1.0, surface(vec3(0.0), 1.0, vec3(0.95, 0.012, 0.032), 0.0));
        SceneObjects.Spheres[1] = sphere(vec3(-3.5, -4.25, 2.25), 0.75, surface(vec3(0.0), 0.7, vec3(0.97, 0.43, 0.012), 0.0));
        SceneObjects.Spheres[2] = sphere(vec3(2.75, -2, 1.25), 1.5, surface(vec3(0.0), 1.0, vec3(1), 0.75));

        SceneObjects.Boxes[0] = box(vec3(5.0, 0.5, 5.0), matr4 {}, vec3(0.0, 5.5, 0.0), surface(vec3(0.0), 0.0, vec3(1.0, 1.0, 1.0), 0));
        SceneObjects.Boxes[1] = box(vec3(5.0, 0.5, 5.0), matr4 {}, vec3(0.0, -5.5, 0.0), surface(vec3(0.0), 0, vec3(1), 0));
        SceneObjects.Boxes[2] = box(vec3(5, 0.5, 5), matr4(
            0.0, 1.0, 0.0, 0,
            -1.0, 0.0, 0.0, 0,
            0.0, 0.0, 1.0, 0,
            0, 0, 0, 1
        ), vec3(5.5, 0.0, 0.0), surface(vec3(0), 0, vec3(0.09, 0.09, 0.91), 0));
        SceneObjects.Boxes[3] = box(vec3(5.0, 0.5, 5.0), matr4(
            0.0, 1.0, 0.0, 0,
            -1.0, 0.0, 0.0, 0,
            0.0, 0.0, 1.0, 0,
            0, 0, 0, 1
        ), vec3(-5.5, 0.0, 0.0), surface(vec3(0.0), 0, vec3(0.94, 0.05, 0.05), 0));
        SceneObjects.Boxes[4] = box(vec3(5.0, 0.5, 5.0), matr4(
            1.0, 0.0, 0.0, 0,
            0.0, 0.0, 1.0, 0,
            0.0, 1.0, 0.0, 0,
            0, 0, 0, 1
        ), vec3(0.0, 0.0, -5.5), surface(vec3(0.0), 0, vec3(1.0, 1.0, 1.0), 0));
        SceneObjects.Boxes[5] = box(vec3(2.5, 0.025, 2.5), matr4(
            1.0, 0.0, 0.0, 0,
            0.0, 1.0, 0.0, 0,
            0.0, 0.0, 1.0, 0,
            0, 0, 0, 1
        ), vec3(0, 4.9, 0), surface(vec3(16), 0, vec3(0), 0));
        SceneObjects.Boxes[6] = box(vec3(1.5, 3.0, 1.5), matr4(
            0.7, 0.0, 0.7, 0,
            0.0, 1.0, 0.0, 0,
            -0.7, 0.0, 0.7, 0,
            0, 0, 0, 1
        ), vec3(-2.0, -2.0, -2.0), surface(vec3(0.0), 0, vec3(0.3), 0));
        SceneObjects.Boxes[7] = box(vec3(1.0, 1.5, 1.0), matr4(
            0.7, 0.0, 0.7, 0,
            0.0, 1.0, 0.0, 0,
            -0.7, 0.0, 0.7, 0,
            0, 0, 0, 1
        ), vec3(2.5, -3.5, -0.0), surface(vec3(0.0), 0, vec3(1.0), 0));
        SceneObjects.Boxes[8] = box(vec3(1.75, 4.75, 0.25), matr4(
            1.0, 0.0, 0.0,0,
            0.0, 1.0, 0.0,0,
            0.0, 0.0, 1.0,0,
            0, 0, 0, 1
        ), vec3(1.5, 0, -5.2), surface(vec3(0.0), 1, vec3(0.85), 0));

        SceneObjectsBuffer->Update(&SceneObjects, sizeof(scene_objects));
    }

    void InitializeSpheresFieldScene()
    {
        SceneObjects.SpheresCount = 64;
        SceneObjects.PlanesCount = 1;

        srand(30);

        SceneObjects.Planes[0] = plane(0, vec3(0, 1, 0), surface(vec3(0), 0, vec3(0.61961, 0.65098, 0.54902), 0));

        int dispersion = (int)sqrt(SceneObjects.SpheresCount) * 3 / 2;
        int ind = 0;
        for (int i = -dispersion; i < dispersion; i += 3)
            for (int j = -dispersion; j < dispersion; j += 3)
            {
                float mat_choose = math::Rnd0<float>();
                vec3 emm, refl;
                float opac, rough;
                if (mat_choose > 0.9)
                {
                    // transparent
                    emm = vec3(0);
                    refl = vec3::Rnd(0.8, 1);
                    opac = math::Rnd<float>(0.75, 1);
                    rough = 1;
                }
                else if (mat_choose > 0.75)
                {
                    // Light emmiter
                    emm = vec3::Rnd(0.1, 6);
                    refl = vec3(1);
                    opac = 0;
                    rough = math::Rnd0<float>();
                }
                else if (mat_choose > 0.375)
                {
                    // reflect
                    emm = vec3(0);
                    refl = vec3::Rnd(0.8, 1);
                    opac = 0;
                    rough = math::Rnd<float>(0.85, 1);
                }
                else
                {
                    // rought
                    emm = vec3(0);
                    refl = vec3::Rnd(0, 1);
                    opac = 0;
                    rough = math::Rnd<float>(0, 0.6);
                }
        
                float radius = math::Rnd(0.1, 1.0);
                float scetter_x = math::Rnd(-0.5, 0.5);
                float scetter_z = math::Rnd(-0.5, 0.5);
                SceneObjects.Spheres[ind++] = sphere(vec3(i + scetter_x, radius, j + scetter_z), radius,
                                                     surface(emm, rough, refl, opac));
            }

        SceneObjectsBuffer->Update(&SceneObjects, sizeof(scene_objects));
    }
};

scl::application *scl::CreateApplication()
{
    return new raytracing_app();
}
