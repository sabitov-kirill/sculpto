/*****************************************************************//**
 * \file   timer.h
 * \brief  Timer, calculating interframe deltatime and fps
 *         class definition module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    using namespace std::chrono;
    class timer
    {
        using time_point = time_point<high_resolution_clock>;

    private: /* Timer data. */
        bool           IsPause {};
        float          Time {};
        float          DeltaTime {};
        float          Fps { -1 };

        time_point     StartTime {};                    /* Time since program start */
        time_point     OldTime {};                      /* Previous response call time */
        time_point     LastFpsCalcutionTime {};         /* Time passed since previous FPS calculation*/
        u32            LastFpsCalcutionFramesCount {};  /* Rendered frames since previous FPS calculation counter. */

        static shared<timer> GlobalTimerInstance;

    public: /* Timer getter/setter functions. */
        static const float UpdateDelay; /* Timer update delay in seconds. */

        /* Is timer paused flag getter function. */
        static bool GetIsPause() { return GlobalTimerInstance->IsPause; }
        /* Current time since 1970 1st Jan getter function. */
        static float GetTime() { return GlobalTimerInstance->Time; }
        /* Inter-frame delta time getter function. */
        static float GetDeltaTime() { return GlobalTimerInstance->DeltaTime; }
        /* Frames per second count getter function. */
        static float GetFps() { return GlobalTimerInstance->Fps; }

        /* Is timer paused flag setter function. */
        static void SetIsPause(bool IsPause) { GlobalTimerInstance->IsPause = IsPause; }
        /* Is timer paused flag toffle function. */
        static void SwitchIsPause() { GlobalTimerInstance->IsPause = !GlobalTimerInstance->IsPause; }

    public:
        /* Timer default constructor. */
        timer();

        /**
         * Timer response function.
         * Calculate inter-frame delta time each call,
         * but FPS only once second.
         * 
         * \param None.
         * \return None.
         */
        void Response();

        /**
         * Global timer getter function.
         * 
         * \param None.
         * \return None.
         */
        static shared<timer> &Get() { return GlobalTimerInstance; }
    };
}
