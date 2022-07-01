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
        float          Fps { 500 };

        time_point     StartTime {};                    /* Time since program start */
        time_point     OldTime {};                      /* Previous response call time */
        time_point     LastFpsCalcutionTime {};         /* Time passed since previous FPS calculation*/
        u32            LastFpsCalcutionFramesCount {};  /* Rendered frames since previous FPS calculation counter. */

    public: /* Timer getter/setter functions. */
        /* Is timer paused flag getter function. */
        bool GetIsPause() const { return IsPause;  }
        /* Current time since 1970 1st Jan getter function. */
        float GetTime() const { return Time; }
        /* Inter-frame delta time getter function. */
        float GetDeltaTime() const { return DeltaTime; }
        /* Frames per second count getter function. */
        float GetFps() const { return Fps; }

        /* Is timer paused flag setter function. */
        void SetIsPause(bool IsPause) { this->IsPause = IsPause; }
        /* Is timer paused flag toffle function. */
        void SwitchIsPause() { IsPause = !IsPause; }

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
    };
}
