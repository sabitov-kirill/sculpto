/*!****************************************************************//*!*
 * \file   mesh_component.h
 * \brief  Scene object skybox component class implementation module.
 *
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "core/resources/skybox.h"

namespace scl
{
    /*! Application scene system objects skybox component class. */
    struct skybox_component
    {
        shared<skybox> SkyboxMesh {};

        skybox_component() = default;
        skybox_component(const skybox_component &Other) = default;
        skybox_component(shared<skybox> Mesh) : SkyboxMesh(Mesh) {}
        ~skybox_component() = default;

        operator const shared<skybox> &() const { return SkyboxMesh; }
        operator shared<skybox> &() { return SkyboxMesh; }
    };
}
