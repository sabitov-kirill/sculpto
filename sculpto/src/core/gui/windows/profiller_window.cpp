/*****************************************************************//**
 * \file   profiller_window.cpp
 * \brief  Application profiller GUI window class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <imgui.h>

#include "profiller_window.h"
#include "core/application/timer.h"
#include "core/render/render_bridge.h"

void scl::profiller_window::Draw()
{
    FPSPlotUpdateDelay += timer::GetDeltaTime();
    if (FPSPlotUpdateDelay > timer::UpdateDelay)
    {
        if (FPSList.size() > 50) FPSList.erase(FPSList.begin());
        FPSList.push_back(timer::GetFps());
        FPSPlotUpdateDelay = 0;
    }

    ImGui::Begin("Profiller");
    {
        ImGui::Text("Average framerate %.1f FPS", timer::GetFps());
        ImGui::PlotLines("", FPSList.data(), (u32)FPSList.size(), 0, 0, FLT_MAX, FLT_MAX, { ImGui::GetWindowWidth() - 15.0f, ImGui::GetWindowHeight() - 56.0f });
    }
    ImGui::End();
}
