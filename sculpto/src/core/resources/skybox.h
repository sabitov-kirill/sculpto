/*****************************************************************//**
 * \file   skybox.h
 * \brief  Sky box render resource class definition module.
 * 
 * \author Sabitov Kirill
 * \date   21 July 2022
 *********************************************************************/

#pragma once

#include "mesh.h"
#include "materials/material_skybox.h"
#include "topology/full_screen_quad.h"

namespace scl
{
    /* Sky box class. */
    class skybox: public mesh
    {
    public: /* Sky box methods. */
        /*!*
         * Sky box default constructor.
         * 
         * \param Texture - skybox texture.
         */
        skybox(shared<texture_2d> Texture) : mesh(topology::full_screen_quad(), material_skybox::Create(Texture)) {}

        /*!*
         * Sky box default constructor.
         *
         * \param Texture - skybox texture.
         */
        static shared<skybox> Create(shared<texture_2d> Texture)
        {
            return CreateShared<skybox>(Texture);
        }
    };
}
