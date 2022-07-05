/*****************************************************************//**
 * \file   gui_layer.cpp
 * \brief  Application graphics user interface layer class implementation module.
 *         Handles user GUI events and widgets render.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_win32.h>

#include "gui_layer.h"
#include "platform/opengl/gl.h"
#include "../application/application.h"
#include "../render/render_context.h"

void scl::gui_layer::OnInit()
{
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();

    // Setting ImGui backend flags
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable &&    // Enable Multi-Viewport / Platform Windows / Not OpenGL
        render_context::GetApi() != render_context::api::OpenGL;

    // Configuring ImGui styles
    SetUpTheme();

    // Initialise platform depended ImGui context
#ifdef SCL_PLATFORM_WINDOWS
    const application &app = application::Get();
    ImGui_ImplWin32_Init(app.GetWindow().GetHandle());
    SCL_CORE_INFO("ImGui Win32 context initialised.");
#else
#   error Other platforms currently dont support GUI
#endif

    // Initialise ImGui render api dependent context
    switch (render_context::GetApi())
    {
    case render_context::api::OpenGL:
        ImGui_ImplOpenGL3_Init("#version 450 core");
        SCL_CORE_INFO("ImGui renderer initialised.");
        break;
    case render_context::api::DirectX: SCL_CORE_ASSERT(0, "DirectX currently dont support GUI"); return;
    default: SCL_CORE_ASSERT(0, "Other platforms currently dont support GUI"); return;
    }
}

void scl::gui_layer::OnClose()
{
    // Deinitialise render context dependent ImGui Implementation
    switch (render_context::GetApi())
    {
    case render_context::api::OpenGL:
        ImGui_ImplOpenGL3_Shutdown();
        SCL_CORE_INFO("ImGui renderer deinitialised.");
        break;
    case render_context::api::DirectX: SCL_CORE_ASSERT(0, "DirectX currently dont support GUI"); return;
    default: SCL_CORE_ASSERT(0, "Other platforms currently dont support GUI"); return;
    }

    // Deinitialise ImGui render api dependent context
#ifdef SCL_PLATFORM_WINDOWS
    ImGui_ImplWin32_Shutdown();
    SCL_CORE_INFO("ImGui Win32 context deinitialised.");
#else
#   error Other platforms currently dont support GUI
#endif

    // Deinitialise ImGui core conext
    ImGui::DestroyContext();
}

void scl::gui_layer::OnEvent(event &Event)
{
    ImGuiIO &io = ImGui::GetIO();
    Event.Handled |= (Event.GetType() == keyboard_event::StaticType) & io.WantCaptureKeyboard;
    Event.Handled |= (Event.GetType() == mouse_button_event::StaticType ||
                      Event.GetType() == mouse_move_event::StaticType) & io.WantCaptureMouse;
}

void scl::gui_layer::RenderGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void scl::gui_layer::SubmitRenderedGui()
{
    ImGuiIO &io = ImGui::GetIO();
    const window::data &win_data = application::Get().GetWindow().GetWindowData();
    io.DisplaySize = ImVec2((float)win_data.Width, (float)win_data.Height);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
#ifdef SCL_PLATFORM_WINDOWS
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
#else
#   error Other platforms currently dont support GUI
#endif
    }
}

void scl::gui_layer::SetUpTheme()
{
    auto& io = ImGui::GetIO();
    float fontSize = 15.0f;
    io.Fonts->AddFontFromFileTTF("assets/fonts/Quicksand-Regular.ttf", fontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Quicksand-Regular.ttf", fontSize);

    auto &style = ImGui::GetStyle();
    style.FrameRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowRounding = 4.0f;
    style.PopupBorderSize = 0.0f;
    style.GrabRounding = 4.0f;

    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.09f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
}
