/*****************************************************************//**
 * \file   camera_component.h
 * \brief  Application scene system object camera component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "core/render/renderer_camera.h"
#include "core/render/primitives/frame_buffer.h"

namespace scl
{
    /* Application scene system object camera component class. */
    struct camera_controller_component
    {
        renderer_camera Camera { camera_projection_type::PERSPECTIVE };
        shared<frame_buffer> MainFrameBuffer { frame_buffer::Create({ 16, 16, 1, false }) };

        camera_controller_component() = default;
        camera_controller_component(const renderer_camera &Camera) : Camera(Camera) {}
        camera_controller_component(const camera_controller_component &Other) = default;
        ~camera_controller_component() = default;

        void Resize(int Width, int Height)
        {
            Camera.Resize(Width, Height);

            frame_buffer_props current_props = MainFrameBuffer->GetFrameBufferProps();
            current_props.Width = Width;
            current_props.Height = Height;
            MainFrameBuffer->SetFrameBufferProps(current_props);
        }
    };
}
