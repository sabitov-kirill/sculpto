/*****************************************************************//**
 * \file   render_adapter.cpp
 * \brief  Render adapter class definition module.
 *         Currently unused render step. Need to applying post-processing,
 *         sybmiting objects from pool rendering, particle system, etc.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "core/application/application.h"
#include "render_adapter.h"

#include "platform/opengl/gl.h"

scl::render_adapter *scl::render_adapter::Instance = nullptr;

void scl::render_adapter::Init()
{
    Renderer = render_context::Create();
    Renderer->Init();

    ViewerCamera.SetLocAtUp(vec3(-3, 10, -3), vec3(0), vec3(0, 1, 0));
}

void scl::render_adapter::Close()
{
    Renderer->Close();
    delete Instance;
}

void scl::render_adapter::RenderFrame()
{
    Renderer->BeginPipeline();
}

void scl::render_adapter::SubmitRenderedFrame()
{
    Renderer->EndPipeline();
    Renderer->SwapBuffers();
}

/**
 * Render context resize call function.
 *
 * \param Width, Height - new width and hight of applicatino window.
 * \return None.
 */
void scl::render_adapter::RenderResize(int Width, int Height)
{
    ViewerCamera.Resize(Width, Height);
    Renderer->Resize(Width, Height);
}
