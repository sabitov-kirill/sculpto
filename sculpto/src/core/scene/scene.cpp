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
    // Set up scene primary camera
    camera *primary_camera {};
    for (auto &&[entity, camera] : Registry.view<camera_component>().each())
        if (camera.IsPrimary) { primary_camera = &camera.Camera; break; }
    if (primary_camera == nullptr) return;
    if (primary_camera->GetViewportWidth() != ViewportWidth || primary_camera->GetViewportHeight() != ViewportHeight)
        primary_camera->Resize(ViewportWidth, ViewportHeight);

    renderer::StartPipeline(*primary_camera, EnviromentAmbient);
    {
        for (auto &&[entity, point_light, transform] : Registry.group<point_light_component>(entt::get<transform_component>).each())
            renderer::SubmitPointLight(transform.Position, point_light.Color * point_light.Strength, point_light.Constant, point_light.Linear, point_light.Quadratic);
    
        for (auto &&[entity, directional_light, transform] : Registry.group<directional_light_component>(entt::get<transform_component>).each())
        {
            vec3 direction = transform.AnglesMatr.TransformVector(vec3 { 0, -1, 0 });
            vec3 at = transform.Position + direction;
            renderer::SubmitDirectionalLight(direction, directional_light.Color * directional_light.Strength, directional_light.GetIsShadow(),
                                             matr4::View(transform.Position, at, { 0, 1, 0 }) * directional_light.GetProjection(), directional_light.GetShadowMap());
        }
    
        for (auto &&[entity, spot_light, transform] : Registry.group<spot_light_component>(entt::get<transform_component>).each())
        {
            vec3 direction = transform.AnglesMatr.TransformVector(vec3 { 0, -1, 0 });
            renderer::SubmitSpotLight(transform.Position, direction, spot_light.Color * spot_light.Strength,
                                      spot_light.InnerCutoffCos, spot_light.OuterCutoffCos, spot_light.Epsilon);
        }
    
        for (auto &&[entity, mesh, transform] : Registry.group<mesh_component>(entt::get<transform_component>).each())
            if (mesh.Mesh != nullptr) renderer::Submit(mesh, transform);
    }
    renderer::EndPipeline();
}


void scl::scene::CallUpdate()
{
    for (auto &&[entity, native_script] : Registry.view<native_script_component>().each())
    {
        // Initialize script component if it is not
        if (!native_script.ObjectBehaviour)
        {
            native_script.ObjectBehaviour = native_script.Instanciate();
            native_script.ObjectBehaviour->Object = scene_object { entity, this };
            native_script.ObjectBehaviour->OnCreate();
        }

        // Call script update function if script is active
        if (native_script.IsActive)
            native_script.ObjectBehaviour->OnUpdate();
    }
}

scl::scene::scene()
{
    event_dispatcher::AddEventListner<mouse_button_event>([&](mouse_button_event &Event) { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<mouse_wheel_event>([&](mouse_wheel_event &Event)   { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<mouse_move_event>([&](mouse_move_event &Event)     { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<keyboard_event>([&](keyboard_event &Event)         { CallUpdate(); return false; });
    event_dispatcher::AddEventListner<viewport_resize_event>([&](viewport_resize_event &Event)
    {
        if (Event.GetViewportId() == ViewportId)
            ViewportWidth = Event.GetWidth(),
            ViewportHeight = Event.GetHeight();
        return false;
    });
}

scl::scene::~scene()
{
}
