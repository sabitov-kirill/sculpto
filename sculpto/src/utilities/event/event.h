/*****************************************************************//**
 * \file   base_event.h
 * \brief  Base event class definiton module.
 * 
 * \author Sabitov Kirill
 * \date   24 June 2022
 *********************************************************************/

#pragma once

#include "sclpch.h"
#include "utilities/string/string_id.h"

#define SCL_MAKE_EVENT(name)                                          \
    public:                                                           \
        static constexpr string_id StaticType = SCL_STRING_ID(#name); \
        inline u32 GetType() const override { return StaticType; }    \
        inline const char *GetName() const override { return #name; }

namespace scl
{
    /* Base event class. */
    class event
    {
    public:
        /* Event handled flag. */
        bool Handled { false };

        /* Defualt destructor. */
        virtual ~event() = default;

        /**
         * Get type of specific event function.
         * 
         * \param None.
         * \return type of specific event.
         */
        inline virtual u32 GetType() const = 0;

        /**
         * Get string name of event.
         *
         * \param None.
         * \return name of event type as string.
         */
        inline virtual const char *GetName() const = 0;

        /**
         * Convert event to string (for debug).
         *
         * \param None.
         * \return string representation of event.
         */
        inline virtual std::string ToString() const { return GetName(); };
    };

    inline std::ostream& operator<<(std::ostream& os, const event& e)
    {
        return os << e.ToString();
    }
}
