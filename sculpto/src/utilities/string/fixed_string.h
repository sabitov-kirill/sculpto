/*!****************************************************************//*!*
 * \file   fixed_string.h
 * \brief  Fixed ;ength string class implementation module.
 *         Useful for class templates.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

namespace scl
{
    /*! Fixed length string class. */
    template <size_t N>
    struct fixed_string
    {
        /*! String characters array. */
        char value[N + 1] {};

        /*! Fiexed string default constructor. */
        constexpr fixed_string() = default;

        /*!*
         * Fixed string constructor bt char array.
         * 
         * \param Str - char array.
         */
        constexpr fixed_string(const char (&Str)[N + 1])
        {
            std::copy_n(value, N + 1, Str);
        }
    };

    /*! Fixed length string class constructor with deduced template. */
    template <size_t Length>
    fixed_string(const char (&str)[Length]) -> fixed_string<Length - 1>;

}
