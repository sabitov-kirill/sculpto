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
#include "core/components/components.h"
#include "core/events/events.h"
#include "core/render/renderer.h"
#include "core/render/render_bridge.h"
#include "core/render/primitives/texture.h"
#include "core/resources/mesh.h"
#include "core/resources/materials/material.h"
#include "core/resources/topology/points.h"
#include "utilities/event/event_dispatcher.h"

const scl::camera_component &scl::scene::GetMainCamera() const
{
    return *MainCamera;
}

void scl::scene::SetMainCamera(const std::string &Name)
{
    for (auto &&[entity, name, camera] : Registry.group<object_name_component>(entt::get<camera_component>).each())
        if (name.Name == Name)
        {
            MainCamera = &camera;
            return;
        }
}

scl::scene_object scl::scene::CreateObject(const std::string &Name)
{
    scene_object created_object { this->Registry.create(), this };
    created_object.AddComponent<object_name_component>(Name == "" ? "Unnamed component" : Name);
    return created_object;
}

void scl::scene::OnUpdate(float DeltaTime)
{
    UpdateDelay += DeltaTime;
    if (UpdateDelay > 0.015)
        CallUpdate();

    Render();
}

void scl::scene::Render()
{
    if (!MainCamera) return;

    renderer::StartPipeline(MainCamera->Camera, EnviromentAmbient);
    {
        for (auto &&[entity, point_light, transform] : Registry.group<point_light_component>(entt::get<transform_component>).each())
            renderer::SubmitPointLight(transform.Position, point_light.Color, point_light.Constant, point_light.Linear, point_light.Quadratic);
    
        for (auto &&[entity, directional_light] : Registry.view<directional_light_component>().each())
        {
            vec3 pos = -directional_light.Direction * directional_light.Distance / 2, at = pos + directional_light.Direction * 5;
            renderer::SubmitDirectionalLight(directional_light.Direction, directional_light.Color, directional_light.IsShadows,
                                             matr4::View(pos, at, { 0, 1, 0 }) * directional_light.Projection, directional_light.ShadowMap);
        }
    
        for (auto &&[entity, spot_light, transform] : Registry.group<spot_light_component>(entt::get<transform_component>).each())
            renderer::SubmitSpotLight(transform.Position, spot_light.Direction, spot_light.Color,
                                      spot_light.InnerCutoffCos, spot_light.OuterCutoffCos, spot_light.Epsilon);
    
        for (auto &&[entity, mesh, transform] : Registry.group<mesh_component>(entt::get<transform_component>).each())
            renderer::Submit(mesh, transform);
    }
    renderer::EndPipeline();
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

        if (native_script.IsActive)
            native_script.ObjectBehaviour->OnUpdate();
    }
}

scl::scene::scene()
{
    // Set up scene event listners
    event_dispatcher::AddEventListner<mouse_button_event>([&](mouse_button_event &Event) { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<mouse_wheel_event>([&](mouse_wheel_event &Event) { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<mouse_move_event>([&](mouse_move_event &Event) { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<keyboard_event>([&](keyboard_event &Event) { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<window_resize_event>([&](window_resize_event &Event)
    {
        ViewportWidth = Event.GetWidth();
        ViewportHeight = Event.GetHeight();
        MainCamera->Camera.Resize(ViewportWidth, ViewportHeight);
        return false;
    });
}

scl::scene::~scene()
{
}
