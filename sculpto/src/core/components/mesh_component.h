/*****************************************************************//**
 * \file   mesh_component.h
 * \brief  Application scene system object mesh component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "../render/mesh.h"

namespace scl
{
    /* Application scene system objects mesh component class. */
    struct mesh_component
    {
        shared<mesh> Mesh;

        mesh_component() = default;
        mesh_component(const mesh_component &Other) = default;
        mesh_component(shared<mesh> Mesh) : Mesh(Mesh) {}
        ~mesh_component() = default;

        operator const shared<mesh> &() const { return Mesh; }
        operator shared<mesh> &() { return Mesh; }
    };
}
