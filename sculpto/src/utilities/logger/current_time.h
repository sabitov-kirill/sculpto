/*!****************************************************************//*!*
 * \file   time_stamp.h
 * \brief  Current time string creation function implementation.
 * 
 * \author Sabitov Kirill
 * \date   23 June 2022
 *********************************************************************/

#pragma once

namespace scl
{
    /*!*
     * Getting curent time string function.
     * 
     * \param None.
     * \return Current time as string in format HH:MM:SS.
     */
    inline const std::string CurrentTime()
    {
        using sysclock_t = std::chrono::system_clock;
        std::time_t now = sysclock_t::to_time_t(sysclock_t::now());
        tm local_time;
        localtime_s(&local_time, &now);

        char buf[16] = { 0 };
        std::strftime(buf, sizeof(buf), "%H:%M:%S", &local_time);

        return std::string(buf);
    }
}
