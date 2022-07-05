/*****************************************************************//**
 * \file   scene.cpp
 * \brief  Application scene sustem class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <entt.hpp>

#include "scene.h"
#include "scene_object.h"
#include "scene_object_behaviour.h"
#include "../components/components.h"
#include "../render/renderer.h"
#include "../events/events.h"
#include "utilities/event/event_dispatcher.h"

void scl::scene::Render()
{
    MainCamera->MainFrameBuffer->Clear();

    renderer::SubmitCamera(MainCamera->Camera);
    for (auto &&[entity, transform, mesh, material] :
         Registry.group<transform_component>(entt::get<mesh_component, mesh_renderer_component>).each())
    {
        renderer::Submit(mesh, material, transform);
    }

    // TODO: Mulitple render passes.

    renderer::Flush(MainCamera->MainFrameBuffer);
}

void scl::scene::CallUpdate()
{
    for (auto &&[entity, native_script] : Registry.view<native_script_component>().each())
    {
        if (!native_script.ObjectBehaviour)
        {
            native_script.ObjectBehaviour = native_script.Instanciate();
            native_script.ObjectBehaviour->Object = scene_object { entity, this };
            native_script.ObjectBehaviour->OnCreate();
        }

        native_script.ObjectBehaviour->OnUpdate();
    }
}

scl::scene::scene()
{
}

scl::scene::~scene()
{
}

scl::scene_object scl::scene::CreateObject(const std::string &Name)
{
    scene_object created_object { this->Registry.create(), this };
    created_object.AddComponent<object_name_component>(Name == "" ? "Unnamed component" : Name);
    return created_object;
}

void scl::scene::OnUpdate(float DeltaTime)
{
    // Update scripts each 0.015sec ~ 60 times per sec
    UpdateDelay += DeltaTime;
    if (UpdateDelay > 0.015)
        CallUpdate(), UpdateDelay = 0;

    // Render objects
    Render();
}

void scl::scene::OnEvent(event &Event)
{
    event_dispatcher dispatcher(Event);
    dispatcher.Dispatch<mouse_button_event>([&](mouse_button_event &Event) { CallUpdate(); return false; });
    dispatcher.Dispatch<mouse_wheel_event>([&](mouse_wheel_event &Event) { CallUpdate(); return false; });
    dispatcher.Dispatch<mouse_move_event>([&](mouse_move_event &Event) { CallUpdate(); return false; });
    dispatcher.Dispatch<keyboard_event>([&](keyboard_event &Event) { CallUpdate();  return false; });
    dispatcher.Dispatch<window_resize_event>([&](window_resize_event &Event)
    {
        ViewportWidth = Event.GetWidth();
        ViewportHeight = Event.GetHeight();
        MainCamera->Resize(ViewportWidth, ViewportHeight);
        return false;
    });
}

void scl::scene::SetMainCamera(const std::string &Name)
{
    for (auto &&[entity, name, camera] :
         Registry.group<object_name_component>(entt::get<camera_controller_component>).each())
    {
        if (name.Name == Name) MainCamera = &camera;
    }
}

void scl::scene::SetRenderToSwapChainBuffer(bool RenderToSwapChainBuffer)
{
    this->RenderToSwapChainBuffer = RenderToSwapChainBuffer;
    frame_buffer_props current_props = MainCamera->MainFrameBuffer->GetFrameBufferProps();
    current_props.SwapChainTarget = RenderToSwapChainBuffer;
    MainCamera->MainFrameBuffer->SetFrameBufferProps(current_props);
}