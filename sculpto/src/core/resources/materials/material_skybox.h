/*****************************************************************//**
 * \file   material_skybox.h
 * \brief  Mesh material for rendering skybox class definition module.
 * 
 * \author Sabitov Kirill
 * \date   21 July 2022
 *********************************************************************/

#pragma once

#include "material.h"
#include "core/render/render_bridge.h"
#include "core/render/primitives/texture.h"

namespace scl
{
    /* Mesh material for rendering skybox class. */
    class material_skybox : public material
    {
    private: /*! Material for blin-phong lighing model data. */
        shared<texture_2d> Texture {};

    public: /*! Material for blin-phong lighing model data getter setter functions. */
        const shared<texture_2d> &GetTexture() const { return Texture; }

        /*! Texture setter function.*/
        void SetTexture(shared<texture_2d> Texture) {
            if (!Texture) return;
            if (this->Texture) this->Texture->Free();
            this->Texture = Texture;
        }

        /*! Default material for blin-phong lighting model constructor. */
        material_skybox() = default;

        /*! Default material data defautl copy constructor. */
        material_skybox(const material_skybox &MaterialData) = default;

        /*!*
         * Material for blin-phong lighting model class contructor.
         *
         * \param Texture - material texture.
         */
        material_skybox(shared<texture_2d> Texture) :
            material(render_bridge::GetSkyboxMaterialShader())
        {
            this->Texture = Texture;
        }

        /*!*
         * Bind material to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override
        {
            render_bridge::SetDepthTestMode(false);
            if (Shader) Shader->Bind();
            if (Texture) Texture->Bind(render_context::TEXTURE_SLOT_MATERIAL_DIFFUSE);
        }

        /*!*
         * Unbind material from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override
        {
            if (Shader) Shader->Unbind();
            if (Texture) Texture->Unbind();
            render_bridge::SetDepthTestMode(true);
        }

        /*!*
         * Material for blin-phong lighting model class contructor.
         *
         * \param Texture - material texture.
         */
        static shared<material_skybox> Create(shared<texture_2d> Texture)
        {
            return CreateShared<material_skybox>(Texture);
        }
    };
}
