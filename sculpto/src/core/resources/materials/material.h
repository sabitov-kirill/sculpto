/*!****************************************************************//*!*
 * \file   material.h
 * \brief  Mesh material class deinition module.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "core/render/primitives/shader.h"
#include "core/render/render_context.h"

namespace scl
{
    /*! Mesh material class. */
    class material
    {
    public: /*! Material data. */
        shared<shader_program> Shader {};

        /*!*
         * Material constructor by shader only.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         */
        material(shared<shader_program> Shader) :
            Shader(Shader) {}

        /*!*
         * Bind material to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const
        {
            if (Shader != nullptr) Shader->Bind();
        }

        /*!*
         * Unbind material from current render stage function.
         *
         * \param Slot - texture slot to unbind it from.
         * \return None.
         */
        virtual void Unbind() const
        {
            if (Shader != nullptr) Shader->Unbind();
        }

        /*!*
         * Material creation function by shader only.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         */
        static shared<material> Create(shared<shader_program> Shader)
        {
            return CreateShared<material>(Shader);
        }
    };
}
