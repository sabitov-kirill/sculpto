/*****************************************************************//**
 * \file   material_phong.h
 * \brief  Mesh material for bling-phone lighting model class deinition module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "material.h"
#include "../primitives/texture.h"
#include "../primitives/buffer.h"

namespace scl
{
    /* Material for blin-phong lighting model class. */
    class material_phong : public material
    {
    private: /* Material for blin-phong lighing model data. */
        vec3 Diffuse {};    /* Material diffuse lighting coefficient. */
        vec3 Specular {};   /* Material specular lighting coefficient. */
        float Shininess {}; /* Material shiness exponent lighting coefficient. */

        /* Material textures cpefficients maps. */
        bool IsDiffuseMap {};
        bool IsSpecularMap {};
        bool IsNormalMap {};
        shared<texture_2d> DiffuseMapTexture {};
        shared<texture_2d> SpecularMapTexture {};
        shared<texture_2d> NormalMapTexture {};

    public: /* Material for blin-phong lighing model data getter setter functions. */
        /* Shiness exponent setter function. */
        void SetShininess(float Shininess) {
            this->Shininess = Shininess;
        }
        /* Diffuse coeffisient setter function.*/
        void SetDiffuse(const vec3 &Diffuse) {
            if (DiffuseMapTexture) DiffuseMapTexture->Free();
            IsDiffuseMap = false;
            this->Diffuse = Diffuse;
        }
        /* Specular coefficient setter function. */
        void SetSpecular(const vec3 &Specular) {
            if (SpecularMapTexture) SpecularMapTexture->Free();
            IsSpecularMap = false;
            this->Specular = Specular;
        }

        /* Diffuse map setter function. */
        void SetDiffuseMapTexture(shared<texture_2d> DiffuseMapTexture) {
            if (this->DiffuseMapTexture) this->DiffuseMapTexture->Free();
            this->DiffuseMapTexture = DiffuseMapTexture;
            IsDiffuseMap = true;
        }
        /* Specular map setter function. */
        void SetSpecularMapTexture(shared<texture_2d> SpecularMapTexture) {
            if (this->SpecularMapTexture) this->SpecularMapTexture->Free();
            this->SpecularMapTexture = SpecularMapTexture;
            IsSpecularMap = true;
        }
        /* Normal map setter function. */
        void SetNormalMapTexture(shared<texture_2d> NormalMapTexture) {
            if (this->NormalMapTexture) this->NormalMapTexture->Free();
            this->NormalMapTexture = NormalMapTexture;
            IsNormalMap = true;
        }

        /* Default material for blin-phong lighting model constructor. */
        material_phong() = default;

        /* Default material data defautl copy constructor. */
        material_phong(const material_phong &MaterialData) = default;

        /**
         * Material constructor by shader only.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         */
        material_phong(shared<shader_program> Shader) :
            material(Shader) {}

        /**
         * Material for blin-phong lighting model class contructor.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param Diffuse  - Material diffuse lighting coefficient.
         * \param Specular - Material specular lighting coefficient.
         * \param Shininess  - Material shiness exponent lighting coefficient.
         */
        material_phong(shared<shader_program> Shader, const vec3 &Diffuse, const vec3 &Specular, float Shininess) :
            material(Shader), Diffuse(Diffuse), Specular(Specular), Shininess(Shininess) {}

        /**
         * Bind material to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override
        {
            if (Shader != nullptr) Shader->Bind();

            // Set specular/specular map
            if (!IsSpecularMap) Shader->SetFloat3(render_context::BINDING_POINT_MATERIAL_DATA + 0, Specular);
            else                SpecularMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_SPECULAR);
            Shader->SetBool(render_context::BINDING_POINT_MATERIAL_DATA + 1, IsSpecularMap);

            // Set diffuse/diffuse map
            if (!IsDiffuseMap) Shader->SetFloat3(render_context::BINDING_POINT_MATERIAL_DATA + 2, Diffuse);
            else               DiffuseMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_DIFFUSE);
            Shader->SetBool(render_context::BINDING_POINT_MATERIAL_DATA + 3, IsDiffuseMap);

            // Set shininess exponent
            Shader->SetFloat(render_context::BINDING_POINT_MATERIAL_DATA + 4, Shininess);

            // Set normal map
            if (IsNormalMap) NormalMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_NORMAL_MAP);
            Shader->SetFloat(render_context::BINDING_POINT_MATERIAL_DATA + 5, IsNormalMap);
        }

        /**
         * Unbind material from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override
        {
            if (Shader != nullptr) Shader->Unbind();
            if (IsSpecularMap) SpecularMapTexture->Unbind();
            if (IsDiffuseMap) DiffuseMapTexture->Unbind();
            if (IsNormalMap) NormalMapTexture->Unbind();
        }

        /**
         * Material creation function by shader only.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \return created material pointer.
         */
        static shared<material_phong> Create(shared<shader_program> Shader)
        {
            return CreateShared<material_phong>(Shader);
        }

       /**
        * Material for blin-phong lighting model class contructor.
        *
        * \param Shader - material shader to use while mesh with this material rendering.
        * \param Specular - Material specular lighting coefficient.
        * \param Diffuse - Material diffuse lighting coefficient.
        * \param Shininess - Material shiness exponent lighting coefficient.
        */
        static shared<material_phong> Create(shared<shader_program> Shader, const vec3 &Diffuse, const vec3 &Specular, float Shininess)
        {
            return CreateShared<material_phong>(Shader, Diffuse, Specular, Shininess);
        }
    };
}
