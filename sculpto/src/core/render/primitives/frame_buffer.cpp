/*!****************************************************************//*!*
 * \file   frame_buffer.h
 * \brief  Frame buffer interface implementation module.
 *
 * \author Sabitov Kirill
 * \date   30 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "../render_context.h"
#include "frame_buffer.h"
#include "platform/opengl/gl_frame_buffer.h"

scl::shared<scl::frame_buffer> scl::frame_buffer::Create(const frame_buffer_props &Props)
{
    switch (render_context::GetApi())
    {
    case scl::render_context_api::OpenGL:  return CreateShared<gl_frame_buffer>(Props);
    case scl::render_context_api::DirectX: SCL_CORE_ASSERT(0, "This API is currently unsupported."); return nullptr;
    }

    SCL_CORE_ASSERT(0, "Unknown render API was selected.");
    return nullptr;
}
