/*****************************************************************//**
 * \file   logger.h
 * \brief  Sculpto library logger implementation module.
 * 
 * \author Sabitov Kirill
 * \date   23 June 2022
 *********************************************************************/

#pragma once

#include <string>

#include "console_colors.h"
#include "current_time.h"

namespace scl
{
    /* Logger class. */
    class logger
    {
    private: /* Logger data. */
        /* Name to be displayed in each log record. */
        std::string Name = "";
        /* Currently using stream. */
        std::ostream &Out = std::cout;

    public:
        /**
         * Logger default constructor.
         * 
         * \param Name - name of logger to be displayed at records.
         */
        logger(const std::string &Name = "");

    public: /* Events logging methods. */
        /**
         * Log info message to current out stream.
         * 
         * \param Message - message to log.
         * \return None.
         */
        template <typename... Targs>
        void Info(std::string_view Format, Targs&&... Args) const
        {
            Out <<
                "[" << CurrentTime() << "] {|" << Name << "|} " <<
                std::format(Format, std::forward<Targs>(Args)...) <<
                '\n';
        }

        /**
         * Log success message to current out stream.
         *
         * \param Message - message to log.
         * \return None.
         */
        template <typename... Targs>
        void Success(std::string_view Format, Targs&&... Args) const
        {
            Out <<
                "[" << CurrentTime() << "] {|" << Name << "|} " <<
                console::color_literal(console::color::GREEN) <<
                std::format(Format, std::forward<Targs>(Args)...) <<
                console::color_literal_reset() << '\n';
        }

        /**
         * Log warn message to current out stream.
         *
         * \param Message - message to log.
         * \return None.
         */
        template <typename... Targs>
        void Warn(std::string_view Format, Targs&&... Args) const
        {
            Out <<
                "[" << CurrentTime() << "] {|" << Name << "|} " <<
                console::color_literal(console::color::YELLOW) <<
                std::format(Format, std::forward<Targs>(Args)...) <<
                console::color_literal_reset() << '\n';
        }

        /**
         * Log error message to current out stream.
         *
         * \param Message - message to log.
         * \return None.
         */
        template <typename... Targs>
        void Error(std::string_view Format, Targs&&... Args) const
        {
            Out <<
                "[" << CurrentTime() << "] {|" << Name << "|} " <<
                console::color_literal(console::color::RED) <<
                std::format(Format, std::forward<Targs>(Args)...) <<
                console::color_literal_reset() << '\n';
        }

        /**
         * Set currently using out stream.
         * 
         * \param OutStream - new stream to log in.
         * \return None.
         */
        void SetStream(std::ostream OutStream);
    };
}
