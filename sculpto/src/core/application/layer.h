/*****************************************************************//**
 * \file   layer.h
 * \brief  Base abstract application layer class definition module.
 *         Layers are stored in stack.
 *         Updated one by one, ordered by time of pushing to stack.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include "utilities/event/event.h"

namespace scl
{
    /* Base abstract application layer class. */
    class layer abstract
    {
    public:
        /* Default layer constructor. */
        layer() = default;

        /* Default layer destcuctor. */
        virtual ~layer() {}

        /**
         * Layer initialisation virtual callback function.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnInit() {}

        /**
         * Layer deinitialisation virtual callback function.
         *
         * \param None.
         * \return None.
         */
        virtual void OnClose() {}

        /**
         * Layer update virtual callback function.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnUpdate(float DeltaTime) {}

        /**
         * Layer GUI rendering virtual callback function.
         * 
         * \param None.
         * \return None.
         */
        virtual void OnGuiRender() {}
    };
}
