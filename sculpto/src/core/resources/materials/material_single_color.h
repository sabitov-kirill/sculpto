/*****************************************************************//**
 * \file   material_phong.h
 * \brief  Mesh material for bling-phone lighting model class deinition module.
 *
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "material.h"
#include "core/render/render_bridge.h"
#include "core/render/primitives/texture.h"
#include "core/render/primitives/buffer.h"

namespace scl
{
    /* Material for blin-phong lighting model class. */
    class material_single_color: public material
    {
    private: /* Material for blin-phong lighing model data. */
        /* Structure to use ass shader buffer data. */
        struct buffer_data
        {
            vec3   Color {};       /* Material color. */
            u32    IsTexture {};   /* Material texture existance flag. */
        } Data {};

        shared<constant_buffer> DataBuffer {};
        shared<texture_2d> Texture {};

    public: /* Material for blin-phong lighing model data getter setter functions. */
        /* Material color getter function. */
        const vec3 &GetColor() const { return Data.Color; }
        /* Material texture existance flag getter function. */
        u32 GetIsTexture() const { return Data.IsTexture; }

        const shared<texture_2d> &GetTexture() const { return Texture; }

        /* Color setter function. */
        void SetColor(const vec3 &Color) {
            if (Texture)
            {
                Texture->Free();
                Texture = nullptr;
            }
            Data.IsTexture = false;
            Data.Color = Color;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /* Texture setter function.*/
        void SetTexture(shared<texture_2d> Texture) {
            if (this->Texture) this->Texture->Free();
            this->Texture = Texture;
            if (!Data.IsTexture)
            {
                Data.IsTexture = true;
                DataBuffer->Update(&Data, sizeof(Data));
            }
        }

        /* Default material for blin-phong lighting model constructor. */
        material_single_color() = default;

        /* Default material data defautl copy constructor. */
        material_single_color(const material_single_color &MaterialData) = default;

        /**
         * Material for blin-phong lighting model class contructor.
         *
         * \param Color - material color.
         */
        material_single_color(const vec3 &Color) :
            material(render_bridge::GetSingleColorMaterialShader())
        {
            Data.Color = Color;
            DataBuffer = constant_buffer::Create(&Data, sizeof(buffer_data));
        }

        /**
         * Material for blin-phong lighting model class contructor.
         *
         * \param Texture - material texture.
         */
        material_single_color(shared<texture_2d> Texture) :
            material(render_bridge::GetSingleColorMaterialShader())
        {
            this->Texture = Texture;
            Data.IsTexture = true;
            DataBuffer = constant_buffer::Create(&Data, sizeof(buffer_data));
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
            if (DataBuffer != nullptr) DataBuffer->Bind(render_context::BINDING_POINT_MATERIAL_DATA);
            if (Data.IsTexture) Texture->Bind(render_context::TEXTURE_SLOT_MATERIAL_DIFFUSE);
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
            if (Data.IsTexture) Texture->Unbind();
        }

        /**
         * Material for blin-phong lighting model class contructor.
         *
         * \param Color - material color.
         */
        static shared<material_single_color> Create(const vec3 &Color)
        {
            return CreateShared<material_single_color>(Color);
        }

        /**
         * Material for blin-phong lighting model class contructor.
         *
         * \param Texture - material texture.
         */
        static shared<material_single_color> Create(shared<texture_2d> Texture)
        {
            return CreateShared<material_single_color>(Texture);
        }
    };
}

