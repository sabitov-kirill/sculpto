/*!****************************************************************//*!*
 * \file   console_colors.h
 * \brief  Helper to setting color of text in console implementation module.
 * 
 * \author Sabitov Kirill
 * \date   23 June 2022
 *********************************************************************/

#pragma once

namespace scl::console
{
    /*! Enum type represending console color code. */
    enum class color
    {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
    };

    /*!*
     * Creating string literal for changing both foreground console colors.
     *
     * \param ForegroundColor - new console foreground color.
     * \return String literal changing console color.
     */
    inline std::string color_literal(color ForegroundColor)
    {
        int foreground = 30 + (int)ForegroundColor;

        return std::string("\x1b[") + std::to_string(foreground) + "m";
    }

    /*!*
     * Creating string literal for changing both background and foreground console colors.
     *
     * \param ForegroundColor - new console foreground color.
     * \param BackgroundColor - new console background color.
     * \return String literal changing console color.
     */
    inline std::string color_literal(color ForegroundColor,
                                     color BackgroundColor)
    {
        int foreground = 30 + (int)ForegroundColor;
        int backgound = 40 + (int)BackgroundColor;

        return std::string("\x1b[") +
            std::to_string(foreground) + ";" +
            std::to_string(backgound) + "m";
    }

    /*!*
     * Creating string literal for reset console colors.
     *
     * \return String literal reseting console color.
     */
    inline const std::string color_literal_reset()
    {
        return "\x1b[0m";
    }
}
