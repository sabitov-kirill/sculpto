/*****************************************************************//**
 * \file   layers_stack.cpp
 * \brief  Application layers stack implemetation module.
 * 
 * \author Sabitov Kirill
 * \date   26 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "layers_stack.h"

scl::layers_stack::~layers_stack()
{
    for (layer *layer : Layers)
        delete layer;
}

void scl::layers_stack::Push(layer *Layer)
{
    Layer->OnInit();
    Layers.emplace(Layers.begin() + (LayerIndex++), Layer);
}

void scl::layers_stack::PushOverlay(layer *Layer)
{
    Layer->OnInit();
    Layers.emplace_back(Layer);
}

void scl::layers_stack::Pop(layer *Layer)
{
    iterator it = std::find(Layers.begin(), Layers.begin() + LayerIndex, Layer);
    if (it != Layers.end())
    {
        Layers.erase(it);
        LayerIndex--;
    }
    Layer->OnClose();
}

void scl::layers_stack::PopOverlay(layer *Layer)
{
    iterator it = std::find(Layers.begin() + LayerIndex, Layers.end(), Layer);
    if (it != Layers.end()) Layers.erase(it);
    Layer->OnClose();
}
