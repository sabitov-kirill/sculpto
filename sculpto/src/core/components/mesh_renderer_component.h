/*****************************************************************//**
 * \file   mesh_renderer_component.h
 * \brief  Application scene system object mesh component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "../render/material/material.h"

namespace scl
{
    struct mesh_renderer_component
    {
        shared<material> MeshMaterial {};

        mesh_renderer_component() = default;
        mesh_renderer_component(const mesh_renderer_component & Other) : MeshMaterial(Other.MeshMaterial) {}
        mesh_renderer_component(shared<material> MeshMaterial) : MeshMaterial(MeshMaterial) {}
        ~mesh_renderer_component() = default;

        operator const shared<material> &() const { return MeshMaterial; }
        operator shared<material> &() { return MeshMaterial; }
    };
}
