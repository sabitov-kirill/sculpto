/*****************************************************************//**
 * \file   event_dispatcher.h
 * \brief  Event dispatcher class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   25 June 2022
 *********************************************************************/

#pragma once

#include "event.h"

namespace scl
{
    /* Static event dispatcher class.
       Should be created once, then event listners added
       to it and dispatched on event invoke. */
    class static_event_dispatcher
    {
        template <typename T>
        using event_callback = std::function<void (const T &)>;
        using base_event_callback = std::function<void (const event &)>;
        using callback_list = std::vector<base_event_callback>;

    private:
        std::map<string_id, callback_list> EventHandlers {};

    public:
        /**
         * Add event listner to specific event function.
         *
         * \param Event - callback to be called on event dispatch.
         * \return None.
         */
        template <typename T >
        void AddEventListner(const event_callback<T> &EventCallback)
        {
            if (EventHandlers.find(T::StaticType) == EventHandlers.end())
                EventHandlers.emplace(T::StaticType, std::vector { EventCallback });
            else
                EventHandlers[T::StaticType].push_back(EventCallback);
        }

        /**
         * Add event listner to specific event operator overload.
         *
         * \param Event - callback to be called on event dispatch.
         * \return None.
         */
        template <typename T>
        void operator+=(const event_callback<T> &EventCallback)
        {
            if (EventHandlers.find(T::StaticType) == EventHandlers.end())
                EventHandlers.emplace(T::StaticType, std::vector { EventCallback });
            else
                EventHandlers[T::StaticType].push_back(EventCallback);
        }

        /**
         * Dispatch specific event function.
         * Calls all callbacks, listing to that event.
         * 
         * \param Event - event to handle.
         * \return None.
         */
        void Invoke(const event &Event)
        {
            if (EventHandlers.find(Event.GetType()) == EventHandlers.end())
                return;

            for (const auto &callback : EventHandlers[Event.GetType()])
                callback(Event);
        }
    };

    /* Event dispatcher class. */
    class event_dispatcher
    {
    private:
        event &Event;

    public:
        /**
         * Event default constructor.
         * 
         * \param Event - event to be dispatched reference.
         */
        event_dispatcher(event &Event) :
            Event(Event) {}

        template <typename T, typename F>
        bool Dispatch(const F &EventCallback)
        {
            if (Event.GetType() != T::StaticType) return false;

            Event.Handled |= EventCallback(static_cast<T&>(Event));
            return true;
        }
    };
}