/*!****************************************************************//*!*
 * \file   application.cpp
 * \brief
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>

#include "application.h"
#include "../render/render_bridge.h"
#include "../render/renderer.h"

scl::application *scl::application::Instance = nullptr;

scl::application::application(const std::string &Name)
{
    // Application singleton instance initialisation
    SCL_CORE_ASSERT(Instance == nullptr, "Application instance already exist.");
    Instance = this;

    // Application core subsystems initialisation
    scl::log::Init();
    Window = window::Create(700, 400, Name + " (Sculpto application).");
    render_bridge::InitContext();
    // renderer::Initialize();
    gui::Init();

    // Setup event handlers
    event_dispatcher::AddEventListner<close_event>([&](close_event &) { SCL_CORE_INFO("Application close event handled."); IsRunning = false; return false; });
}

scl::application::~application()
{
    // Do nothing in destructor (do nont deinitialise application subsystems)
    // for having oportunity to do it when everything still do not destroed. E.g. 
    // when application destructor called window is already destroyed and Dead ImGui
    // cannot be deintialised properly.
}

void scl::application::LoopIterationActions()
{
    // Update application subsystems
    timer::Get()->Response();
    input_system::Response();

    // Update window
    Window->Update();

    // Do nothing if application is already closed.
    if (!IsRunning) return;

    // Rendering GUI, handle user actions
    if (GuiEnabled)
    {
        gui::BeginUpdate();
        OnGuiUpdate();
        gui::SubmitUpdate();
    }

    // Update application
    OnUpdate(timer::GetDeltaTime());

    render_bridge::SwapBuffers();
}

void scl::application::Run()
{
    // Client application initialisation.
    // Not called in constructor, after all subsystems initilisation, because of
    // imposibility to call virtual functions in constructor.
    // Is happes due to the fact that the table of virtual pointers is created
    // only after the call to the base class constructor has completed
    SCL_CORE_INFO("User application initialisation started");
    this->OnInit();
    SCL_CORE_INFO("User application initialised.");

    // Starting main loop
    SCL_CORE_INFO("Application main loop started.");
    while (IsRunning)
        this->LoopIterationActions();

    SCL_CORE_INFO("Application main loop ended.");

    // Deisnitalise application subsystems
    this->OnClose();
    gui::Close();
    render_bridge::CloseContext();
}

void scl::application::ShutDown()
{
    SCL_CORE_INFO("Application shut down function called.");
    Window->ShutDown();
}
