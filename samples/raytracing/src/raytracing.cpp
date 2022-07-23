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
    struct scene_data
    {
        vec3 CameraPosition;
        float CameraProjectionDistance;
        vec3 CameraDirection;
        float ViewportWidth;
        vec3 CameraRightDirection;
        float ViewportHeight;
        vec3 CameraUpDirection;
        float ViewportProjectionWidth;
        float ViewportProjectionHeight;
        float Time;
        u32 Samples;
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
    scene_data SceneData {};
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
};

scl::application *scl::CreateApplication()
{
    return new raytracing_app();
}
