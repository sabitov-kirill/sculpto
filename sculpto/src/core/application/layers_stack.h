/*****************************************************************//**
 * \file   layers_stack.h
 * \brief  Application layers stack class definition module.
 *
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#pragma once

#include "layer.h"

namespace scl
{
    /* Application layers stack class. */
    class layers_stack
    {
        using layers_list = std::vector<layer *>;
        using iterator = layers_list::iterator;
        using reverse_iterator = layers_list::reverse_iterator;
        using const_iterator = layers_list::const_iterator;
        using const_reverse_iterator = layers_list::const_reverse_iterator;

    private:
        layers_list Layers {};
        u32 LayerIndex = 0;

    public: /* Layers stack kmethods. */
        /* Lasyers stack default constructor. */
        layers_stack() = default;

        /* Lasyers stack default destructor. */
        ~layers_stack();

        /**
         * Push common layer to stack function.
         * 
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void Push(layer *Layer);

        /**
         * Push overlay layer (e.g. GUI) to stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void PushOverlay(layer *Layer);

        /**
         * Pop common layer from stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void Pop(layer *Layer);

        /**
         * Pop overlay layer (e.g. GUI) from stack function.
         *
         * \param Layer - layer to push to stack.
         * \return None.
         */
        void PopOverlay(layer *Layer);

    public: /* Data getters/setters. */
        iterator begin() { return Layers.begin(); }
        iterator end() { return Layers.end(); }
        reverse_iterator rbegin() { return Layers.rbegin(); }
        reverse_iterator rend() { return Layers.rend(); }
        const_iterator cbegin() { return Layers.cbegin(); }
        const_iterator cend() { return Layers.cend(); }
        const_reverse_iterator crbegin() { return Layers.crbegin(); }
        const_reverse_iterator crend() { return Layers.crend(); }
    };
}
