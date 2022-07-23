/*!****************************************************************//*!*
 * \file   timer.h
 * \brief  Timer, calculating interframe deltatime and fps
 *         class implementation module.
 *
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "timer.h"

scl::shared<scl::timer> scl::timer::GlobalTimerInstance = scl::CreateShared<timer>();
const float scl::timer::UpdateDelay = 0.2f;

scl::timer::timer()
{
    LastFpsCalcutionTime = OldTime = StartTime = high_resolution_clock::now();
}

void scl::timer::Response()
{
    time_point current_time = high_resolution_clock::now();
    Time = duration<float>(current_time - StartTime).count();

    LastFpsCalcutionFramesCount++;
    float fps_calculation_delta_time = duration<float>(current_time - LastFpsCalcutionTime).count();
    if (fps_calculation_delta_time >= UpdateDelay)
    {
        Fps = LastFpsCalcutionFramesCount / fps_calculation_delta_time;
        LastFpsCalcutionTime = high_resolution_clock::now();
        LastFpsCalcutionFramesCount = 0;
    }

    DeltaTime = duration<float>(current_time - OldTime).count();
    OldTime = current_time;
}
