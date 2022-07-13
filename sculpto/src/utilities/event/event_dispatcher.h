/*****************************************************************//**
 * \file   event_dispatcher.h
 * \brief  Event dispatcher class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#pragma once

#include "event.h"
#include "utilities/string/string_id.h"

namespace scl
{
    /* Static event dispatcher class.
       Should be created once, then event listners added
       to it and dispatched on event invoke. */
    class event_dispatcher
    {
        template <typename Tevent>
        using event_callback = std::function<bool(Tevent &)>;
        using base_event_callback = std::function<bool(event &)>;
        using callback_list = std::vector<base_event_callback>;

    private: /* Static event dispatcher data. */
        static std::map<string_id, callback_list> EventHandlers;

    public: /* Static event dispatcher methods. */
        /**
         * Add event listner to specific event function.
         *
         * \tparam Tevent - event to set listner to.
         * \param Event - callback to be called on event dispatch.
         * \return None.
         */
        template <typename Tevent>
        static void AddEventListner(base_event_callback &&EventCallback)
        {
            auto event_handlers = EventHandlers.find(Tevent::StaticType);
            if (event_handlers == EventHandlers.end())
                EventHandlers.emplace(Tevent::StaticType, std::vector { std::move(EventCallback) });
            else
                event_handlers->second.push_back(std::move(EventCallback));
        }

        /**
         * Add event listner to specific event function.
         *
         * \tparam Tevent - event to set listner to.
         * \param Event - callback to be called on event dispatch.
         * \return None.
         */
        template <typename Tevent>
        static void AddEventListner(std::function<bool(Tevent &)> &&EventCallback)
        {
            AddEventListner<Tevent>([callback = std::move(EventCallback)](event &Event)
            {
                if (Event.GetType() != Tevent::StaticType) return false;
                return callback(static_cast<Tevent &>(Event));
            });
        }

        /**
         * Dispatch specific event function.
         * Calls all callbacks, listing to that event.
         * 
         * \param Event - event to handle.
         * \return None.
         */
        template <typename Tevent>
        static void Invoke(Tevent &Event)
        {
            std::map<string_id, callback_list>::iterator event_handlers = EventHandlers.find(Event.GetType());
            if (event_handlers == EventHandlers.end()) return;

            for (const auto &callback : event_handlers->second)
            {
                Event.Handled |= callback(Event);
                if (Event.Handled) break;
            }
        }
    };
}