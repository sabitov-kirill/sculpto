/*****************************************************************//**
 * \file   application.cpp
 * \brief
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include "sclpch.h"
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

    // Window initilisation
    Window = window::Create(700, 400, Name + " (Sculpto application).", false);
    Window->SetEventHandler(std::bind_front(&application::EventHandler, this));
    SCL_CORE_SUCCES("Application window created.");

    // Render initialisation
    render_bridge::InitContext();

    // Gui layer initialisation
    GUILayer = new gui_layer();
    PushOverlay(GUILayer);
}

scl::application::~application()
{
    // Do nothing in destructor (do nont deinitialise application subsystems)
    // for having oportunity to do it when everything still do not destroed. E.g. 
    // when application constructor called window is already destroyed and Dead ImGui
    // cannot be deintialised properly.
}

bool scl::application::OnWindowResize(window_resize_event &Event)
{
    render_bridge::ResizeContext(Event.GetWidth(), Event.GetHeight());       // Call resize to render context
    this->LoopIterationActions();                                            // Forece loop iteration to render frame
    return false;                                                            // Let event be handled by other handlers
}

bool scl::application::OnWindowClose(window_close_event &Event)
{
    IsRunning = false, SCL_CORE_SUCCES("Application closed."); // End main app loop
    this->OnClose();                                           // Client application deinitialisation
    for (layer *layer : Layers) layer->OnClose();              // All layers in stack deinitilisation

    render_bridge::CloseContext();                             // Render deintialisation
    return false;                                              // Let event be handled by other handlers
}

void scl::application::EventHandler(event &Event)
{
    event_dispatcher dispatcher(Event);

    // Dispatch events, refelecting application work.
    dispatcher.Dispatch<window_resize_event>(SCL_BIND_EVENT_FN(OnWindowResize));
    dispatcher.Dispatch<window_close_event>(SCL_BIND_EVENT_FN(OnWindowClose));

    // Do nothing if application is already closed.
    if (!IsRunning) return;

    // Call all layers event handlers in reversed order.
    for (auto &layer : Layers)
    {
        layer->OnEvent(Event);
        if (Event.Handled == true)
            break;
    }
}

void scl::application::LoopIterationActions()
{
    // Do nothing if application is already closed.
    if (!IsRunning) return;

    // Update application subsystems
    timer::Get()->Response();
    input_system::Response();

    // Update all layers
    for (auto it = Layers.rbegin(); it != Layers.rend(); ++it)
        (*it)->OnUpdate(timer::GetDeltaTime());

    // Rendering GUI
    if (GuiEnabled)
    {
        GUILayer->RenderGui();
        {
            for (auto it = Layers.rbegin(); it != Layers.rend(); ++it)
                (*it)->OnGuiRender();
        }
        GUILayer->SubmitRenderedGui();
    }

    render_bridge::SwapBuffers();
}

void scl::application::Run()
{
    // Client application initialisation.
    // Not called in constructor, after all subsystems initilisation, because of
    // imposibility to call virtual functions in constructor.
    // Is happes due to the fact that the table of virtual pointers is created
    // only after the call to the base class constructor has completed
    this->OnInit();

    // Starting main loop
    SCL_CORE_SUCCES("Application main loop started.");
    while (IsRunning)
    {
        Window->Update();
        this->LoopIterationActions();
    }
}

void scl::application::ShutDown()
{
    Window->ShutDown();
}
