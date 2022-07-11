/*****************************************************************//**
 * \file   editor_viewport_window.h
 * \brief  Sculpto editor viewport window class implementation module.
 *
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>
#include <entt.hpp>

#include "viewport_window.h"
#include "core/scene/scene.h"
#include "core/events/window_event.h"
#include "core/render/primitives/texture.h"
#include "core/render/primitives/frame_buffer.h"

scl::viewport_window::viewport_window(shared<frame_buffer> ViewportBuffer, bool ResizeBuffer, scene *Scene) :
    ViewportBuffer(ViewportBuffer), ResizeBuffer(ResizeBuffer), Scene(Scene) {}

void scl::viewport_window::Draw()
{
    ImGui::Begin("Viewport");
    {
        const ImVec2 window_size = ImGui::GetContentRegionAvail();
        const frame_buffer_props &viewport_buffer_props = ViewportBuffer->GetFrameBufferProps();

        if (ResizeBuffer && (viewport_buffer_props.Width != window_size.x || viewport_buffer_props.Height != window_size.y) &&
            window_size.x > 0 && window_size.y > 0)
            if (Scene)
            {
                // Call scene on event function to dispatche window resize callback
                window_resize_event e { (int)window_size.x, (int)window_size.y };
                Scene->OnEvent(e);
            }
            else
            {
                // Just resize viewport framebuffer
                frame_buffer_props new_viewport_buffer_props = viewport_buffer_props;
                new_viewport_buffer_props.Width  = window_size.x;
                new_viewport_buffer_props.Height = window_size.y;
                ViewportBuffer->SetFrameBufferProps(new_viewport_buffer_props);
            }

        // Render framebuffer
        ImGui::Image((ImTextureID)ViewportBuffer->GetColorAttachment(0)->GetHandle(),
                     ImVec2(window_size.x, window_size.y), { 0, 1 }, { 1, 0 });

        if (ImGui::IsItemHovered()) ImGui::CaptureMouseFromApp(false);
    }
    ImGui::End();
}
