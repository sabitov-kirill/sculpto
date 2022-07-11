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
    current_props.IsSwapChainTarget = RenderToSwapChainBuffer;
    MainCamera->MainFrameBuffer->SetFrameBufferProps(current_props);
}

void scl::scene::SetIsHDR(bool IsHDR)
{
    this->IsHDR = IsHDR;
    if (IsHDR && !HDRPassFullscreenQuad)
        HDRPassFullscreenQuad = mesh::Create(topology::points(), material::Create(render_bridge::GetToneMappingPassShader()));
}

void scl::scene::SetIsBloom(bool IsBloom)
{
    if (IsBloom && !IsHDR) SCL_CORE_ERROR("Bloom effect can't be activated if HDR is off.");
    this->IsBloom = IsBloom;
    if (this->IsBloom && !BloomBlurringBuffers[0])
        BloomBlurringBuffers[0] = frame_buffer::Create(frame_buffer_props { ViewportWidth, ViewportHeight, 1, false, 1, 0, true });
    if (this->IsBloom && !BloomBlurringBuffers[1])
        BloomBlurringBuffers[1] = frame_buffer::Create(frame_buffer_props { ViewportWidth, ViewportHeight, 1, false, 1, 0, true });
    if (!this->IsBloom && BloomBlurringBuffers[0])
        BloomBlurringBuffers[0]->Clear();
    if (!this->IsBloom && BloomBlurringBuffers[1])
        BloomBlurringBuffers[1]->Clear();
    if (this->IsBloom && !BloomBluringFullscreenQuad)
        BloomBluringFullscreenQuad = mesh::Create(topology::points(), material::Create(render_bridge::GetGaussianBlurPassShader()));
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
        CallUpdate();

    // Render objects
    Render();
}

void scl::scene::OnEvent(event &Event)
{
    event_dispatcher dispatcher(Event);
    dispatcher.Dispatch<mouse_button_event>([&](mouse_button_event &Event) { CallUpdate(); return false; });
    dispatcher.Dispatch<mouse_wheel_event>([&](mouse_wheel_event &Event)   { CallUpdate(); return false; });
    dispatcher.Dispatch<mouse_move_event>([&](mouse_move_event &Event)     { CallUpdate(); return false; });
    dispatcher.Dispatch<keyboard_event>([&](keyboard_event &Event)         { CallUpdate();  return false; });
    dispatcher.Dispatch<window_resize_event>([&](window_resize_event &Event)
    {
        ViewportWidth = Event.GetWidth();
        ViewportHeight = Event.GetHeight();
        MainCamera->Resize(ViewportWidth, ViewportHeight);

        auto pr = BloomBlurringBuffers[0]->GetFrameBufferProps();
        pr.Width = Event.GetWidth();
        pr.Height = Event.GetHeight();
        BloomBlurringBuffers[0]->SetFrameBufferProps(pr);
        BloomBlurringBuffers[1]->SetFrameBufferProps(pr);
        return false;
    });
}

void scl::scene::Render()
{
    if (!MainCamera) return;

    // Submit lights
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

    // Main color pass
    renderer::StartPipeline(MainCamera->Camera.GetViewProjection(), MainCamera->Camera.GetDirection(), MainCamera->Camera.GetPosition(),
                            ViewportWidth, ViewportHeight, EnviromentAmbient, IsBloom);
    for (auto &&[entity, mesh, transform] : Registry.group<mesh_component>(entt::get<transform_component>).each())
        renderer::Submit(mesh, transform);
    renderer::EndPipeline(IsHDR ? MainCamera->HDRFrameBuffer : MainCamera->MainFrameBuffer);

    if (IsHDR && !IsBloom)
    {
        MainCamera->MainFrameBuffer->Clear();
        MainCamera->MainFrameBuffer->Bind();
        HDRPassFullscreenQuad->SubMeshes[0].Material->Bind();
        HDRPassFullscreenQuad->SubMeshes[0].Material->Shader->SetFloat("u_Exposure", Exposure);
        MainCamera->HDRFrameBuffer->GetColorAttachment(0)->Bind(render_context::TEXTURE_SLOT_HDR_BUFFER);
        render_bridge::DrawIndices(HDRPassFullscreenQuad->SubMeshes[0].VertexArray);
        MainCamera->MainFrameBuffer->Unbind();
    }

    if (IsHDR && IsBloom)
    {
        bool horizontal = true, first_iteration = true;
        int amount = 4;
        BloomBluringFullscreenQuad->SubMeshes[0].Material->Shader->Bind();
        for (int i = 0; i < amount; i++)
        {
            BloomBlurringBuffers[horizontal]->Bind();
            BloomBluringFullscreenQuad->SubMeshes[0].Material->Shader->SetBool("u_IsHorizontal", horizontal);
            if (first_iteration) MainCamera->HDRFrameBuffer->GetColorAttachment(1)->Bind(render_context::TEXTURE_SLOT_BLUR_BUFFER);
            else                 BloomBlurringBuffers[!horizontal]->GetColorAttachment(0)->Bind(render_context::TEXTURE_SLOT_BLUR_BUFFER);
            render_bridge::DrawIndices(BloomBluringFullscreenQuad->SubMeshes[0].VertexArray);
            horizontal = !horizontal;
            if (first_iteration) first_iteration = false;
        }
        BloomBlurringBuffers[!horizontal]->Unbind();

        MainCamera->MainFrameBuffer->Clear();
        MainCamera->MainFrameBuffer->Bind();
        HDRPassFullscreenQuad->SubMeshes[0].Material->Bind();
        HDRPassFullscreenQuad->SubMeshes[0].Material->Shader->SetFloat("u_Exposure", Exposure);
        MainCamera->HDRFrameBuffer->GetColorAttachment(0)->Bind(render_context::TEXTURE_SLOT_HDR_BUFFER);
        BloomBlurringBuffers[!horizontal]->GetColorAttachment(0)->Bind(render_context::TEXTURE_SLOT_BLUR_BUFFER);
        render_bridge::DrawIndices(HDRPassFullscreenQuad->SubMeshes[0].VertexArray);
        MainCamera->MainFrameBuffer->Unbind();
    }
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
