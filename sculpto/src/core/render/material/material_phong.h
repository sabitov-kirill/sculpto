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
        /* Structure to use ass shader buffer data. */
        struct buffer_data
        {
            vec3   Specular {};       /* Material specular lighting coefficient. */
            float  Shininess {};      /* Material shiness exponent lighting coefficient. */
            vec3   Diffuse {};        /* Material diffuse lighting coefficient. */
            u32    IsSpecularMap {};  /* Flag, showing whether specular map passing to shader. */
            u32    IsDiffuseMap {};   /* Flag, showing whether diffuse map passing to shader. */
            u32    IsNormalMap {};    /* Flag, showing whether normal map passing to shader. */
        } Data {};

        /* Constant shader buffer for passing material data to shader. */
        shared<constant_buffer> DataBuffer {};

        /* Material textures cpefficients maps. */
        shared<texture_2d> SpecularMapTexture {};
        shared<texture_2d> DiffuseMapTexture {};
        shared<texture_2d> NormalMapTexture {};

    public: /* Material for blin-phong lighing model data getter setter functions. */
        /* Diffuse coeffisient setter function.*/
        void SetDiffuse(const vec3 &Diffuse) {
            if (DiffuseMapTexture) DiffuseMapTexture->Free();
            Data.IsDiffuseMap = false;
            Data.Diffuse = Diffuse;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /* Specular coefficient setter function. */
        void SetSpecular(const vec3 &Specular) {
            if (SpecularMapTexture) SpecularMapTexture->Free();
            Data.IsSpecularMap = false;
            Data.Specular = Specular;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /* Shiness exponent setter function. */
        void SetShininess(float Shininess) {
            Data.Shininess = Shininess;
            DataBuffer->Update(&Data, sizeof(Data));
        }

        /* Diffuse map setter function. */
        void SetDiffuseMapTexture(shared<texture_2d> DiffuseMapTexture) {
            if (this->DiffuseMapTexture) this->DiffuseMapTexture->Free();
            this->DiffuseMapTexture = DiffuseMapTexture;
            Data.IsDiffuseMap = true;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /* Specular map setter function. */
        void SetSpecularMapTexture(shared<texture_2d> SpecularMapTexture) {
            if (this->SpecularMapTexture) this->SpecularMapTexture->Free();
            this->SpecularMapTexture = SpecularMapTexture;
            Data.IsSpecularMap = true;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /* Normal map setter function. */
        void SetNormalMapTexture(shared<texture_2d> NormalMapTexture) {
            if (this->NormalMapTexture) this->NormalMapTexture->Free();
            this->NormalMapTexture = NormalMapTexture;
            Data.IsNormalMap = true;
            DataBuffer->Update(&Data, sizeof(Data));
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
            material(Shader),
            DataBuffer(constant_buffer::Create(render_context::BINDING_POINT_MATERIAL_DATA, nullptr, sizeof(buffer_data))) {}

        /**
         * Material for blin-phong lighting model class contructor.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param Diffuse  - Material diffuse lighting coefficient.
         * \param Specular - Material specular lighting coefficient.
         * \param Shininess  - Material shiness exponent lighting coefficient.
         */
        material_phong(shared<shader_program> Shader, const vec3 &Diffuse, const vec3 &Specular, float Shininess) :
            material(Shader)
        {
            Data.Diffuse = Diffuse;
            Data.Specular = Specular;
            Data.Shininess = Shininess;

            DataBuffer = constant_buffer::Create(render_context::BINDING_POINT_MATERIAL_DATA, &Data, sizeof(buffer_data));
        }

        /**
         * Bind material to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override
        {
            if (Shader != nullptr) Shader->Bind();
            if (DataBuffer != nullptr) DataBuffer->Bind();

            if (Data.IsSpecularMap) SpecularMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_SPECULAR);
            if (Data.IsDiffuseMap) DiffuseMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_DIFFUSE);
            if (Data.IsNormalMap) NormalMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_NORMAL_MAP);
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
            if (DataBuffer != nullptr) DataBuffer->Unbind();
            if (Data.IsSpecularMap) SpecularMapTexture->Unbind();
            if (Data.IsDiffuseMap) DiffuseMapTexture->Unbind();
            if (Data.IsNormalMap) NormalMapTexture->Unbind();
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
