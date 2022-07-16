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
#include "core/events/events.h"
#include "core/render/primitives/frame_buffer.h"
#include "core/render/primitives/texture.h"

scl::viewport_window::viewport_window(const shared<frame_buffer> &ViewportBuffer, int ViewportId) :
    ViewportBuffer(ViewportBuffer) { SetViewportId(ViewportId); }

void scl::viewport_window::Draw()
{
    ImGui::Begin("Viewport");
    {
        const ImVec2 window_size = ImGui::GetContentRegionAvail();
        if ((window_size.x != ViewportWidth || window_size.y != ViewportHeight) &&
             window_size.x > 0 && window_size.y > 0)
            {
                ViewportWidth = window_size.x, ViewportHeight = window_size.y;
                viewport_resize_event e { ViewportWidth, ViewportHeight, ViewportId };
                event_dispatcher::Invoke(e);
            }

        if (auto viewport_buffer = ViewportBuffer.lock())
            ImGui::Image((ImTextureID)viewport_buffer->GetColorAttachment()->GetHandle(), window_size, {0, 1}, {1, 0});

        if (ImGui::IsItemHovered()) ImGui::SetNextFrameWantCaptureMouse(false);
        if (ImGui::IsItemActive()) ImGui::SetNextFrameWantCaptureKeyboard(false);
    }
    ImGui::End();
}
