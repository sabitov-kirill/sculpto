/*!****************************************************************//*!*
 * \file   profiller_window.h
 * \brief  Application profiller GUI window class definition module.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

namespace scl
{
    /*! Application profiller GUI window class. */
    class profiller_window
    {
    private: /*! Application profiller GUI window data. */
        std::vector<float> FPSList {};
        float FPSPlotUpdateDelay {};
        bool CollectProfilingData;

    public:
        /*! Application profiller GUI window default constructor. */
        profiller_window() = default;

        /*!*
         * Draw profiller GUI window function.
         * 
         * \param None.
         * \return None.
         */
        void Draw();
    };
}
