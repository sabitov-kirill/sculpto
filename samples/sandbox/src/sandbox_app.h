/*****************************************************************//**
 * \file   sandbox_app.h
 * \brief  Sandbox application definitions module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include "sculpto.h"

class test_gui_layer: public scl::layer
{
    scl::shared<scl::frame_buffer> FrameBuffer;
    scl::shared<scl::mesh> Cube;
    scl::camera Camera;

    float ObjectSize { 1 };
    float ObjectRotationX { 0 }, ObjectRotationY { 0 }, ObjectRotationZ { 0 };
    scl::vec3 ObjectPosition {};
    scl::vec4 ClearColor {};
    bool IsWireframe {};
    bool IsDockspace { true };

    void OnInit() override;
    void OnUpdate() override;
    void OnGuiRender() override;
};

class sandbox_app: public scl::application
{
public:
    sandbox_app() : application("Sadbox")
    {
        PushOverlay(new test_gui_layer());
    }

    ~sandbox_app() override
    {
    }

    void OnInit() override
    {
    }

    void OnClose() override
    {
    }
};

scl::application *scl::CreateApplication()
{
    return new sandbox_app();
}
