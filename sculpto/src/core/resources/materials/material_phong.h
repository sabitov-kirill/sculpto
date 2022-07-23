/*!****************************************************************//*!*
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
    /*! Material for blin-phong lighting model class. */
    class material_phong : public material
    {
    private: /*! Material for blin-phong lighing model data. */
        /*! Structure to use ass shader buffer data. */
        struct buffer_data
        {
            vec3   Specular {};       /*! Material specular lighting coefficient. */
            float  Shininess {};      /*! Material shiness exponent lighting coefficient. */
            vec3   Diffuse {};        /*! Material diffuse lighting coefficient. */
            u32    IsSpecularMap {};  /*! Flag, showing whether specular map passing to shader. */
            u32    IsDiffuseMap {};   /*! Flag, showing whether diffuse map passing to shader. */
            u32    IsEmissionMap {};  /*! Flag, showing whether normal map passing to shader. */
            u32    IsNormalMap {};    /*! Flag, showing whether normal map passing to shader. */
        } Data {};

        /*! Constant shader buffer for passing material data to shader. */
        shared<constant_buffer> DataBuffer {};

        /*! Material textures cpefficients maps. */
        shared<texture_2d> SpecularMapTexture {};
        shared<texture_2d> DiffuseMapTexture {};
        shared<texture_2d> EmissionMapTexture {};
        shared<texture_2d> NormalMapTexture {};

    public: /*! Material for blin-phong lighing model data getter setter functions. */
        /*! Material specular lighting coefficient getter function. */
        const vec3 &GetSpecular() const { return Data.Specular; }
        /*! Material shiness exponent lighting coefficient getter function. */
        const vec3 &GetDiffuse() const { return Data.Diffuse; }
        /*! Material diffuse lighting coefficient getter function. */
        float GetShininess() const { return Data.Shininess; }

        /*! Flag, showing whether specular map passing to shader getter function. */
        bool GetIsSpecularMap() const { return Data.IsSpecularMap; }
        /*! Flag, showing whether diffuse map passing to shader getter function. */
        bool GetIsDiffuseMap() const { return Data.IsDiffuseMap; }
        /*! Flag, showing whether normal map passing to shader getter function. */
        bool GetIsEmissionMap() const { return Data.IsEmissionMap; }
        /*! Flag, showing whether normal map passing to shader getter function. */
        bool GetIsNormalMap() const { return Data.IsNormalMap; }

        /*! Diffuse map getter function. */
        const shared<texture_2d> &GetDiffuseMapTexture() const { return DiffuseMapTexture; }
        /*! Specular map getter function. */
        const shared<texture_2d> &GetSpecularMapTexture() const { return SpecularMapTexture; }
        /*! Emission map getter function. */
        const shared<texture_2d> &GetEmissionMapTexture() const { return EmissionMapTexture; }
        /*! Normal map getter function. */
        const shared<texture_2d> &GetNormalMapTexture() const { return NormalMapTexture; }

        /*! Diffuse coeffisient setter function.*/
        void SetDiffuse(const vec3 &Diffuse) {
            if (DiffuseMapTexture)
            {
                DiffuseMapTexture->Free();
                DiffuseMapTexture = nullptr;
            }
            Data.Diffuse = Diffuse;
            Data.IsDiffuseMap = false;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /*! Specular coefficient setter function. */
        void SetSpecular(const vec3 &Specular) {
            if (SpecularMapTexture)
            {
                SpecularMapTexture->Free();
                SpecularMapTexture = nullptr;
            }
            Data.Specular = Specular;
            Data.IsSpecularMap = false;
            DataBuffer->Update(&Data, sizeof(Data));
        }
        /*! Shiness exponent setter function. */
        void SetShininess(float Shininess) {
            Data.Shininess = Shininess;
            DataBuffer->Update(&Data, sizeof(Data));
        }

        /*! Diffuse map setter function. */
        void SetDiffuseMapTexture(shared<texture_2d> DiffuseMapTexture) {
            if (!DiffuseMapTexture) return;

            if (this->DiffuseMapTexture) this->DiffuseMapTexture->Free();
            this->DiffuseMapTexture = DiffuseMapTexture;
            if (!Data.IsDiffuseMap)
            {
                Data.IsDiffuseMap = true;
                DataBuffer->Update(&Data, sizeof(Data));
            }
        }
        /*! Specular map setter function. */
        void SetSpecularMapTexture(shared<texture_2d> SpecularMapTexture) {
            if (!SpecularMapTexture) return;

            if (this->SpecularMapTexture) this->SpecularMapTexture->Free();
            this->SpecularMapTexture = SpecularMapTexture;
            if (!Data.IsSpecularMap)
            {
                Data.IsSpecularMap = true;
                DataBuffer->Update(&Data, sizeof(Data));
            }
        }
        /*! Emission map setter function. */
        void SetEmissionMapTexture(shared<texture_2d> EmissionMapTexture) {
            if (!EmissionMapTexture) return;

            if (this->EmissionMapTexture) this->EmissionMapTexture->Free();
            this->EmissionMapTexture = EmissionMapTexture;
            if (!Data.IsEmissionMap)
            {
                Data.IsEmissionMap = true;
                DataBuffer->Update(&Data, sizeof(Data));
            }
        }
        /*! Normal map setter function. */
        void SetNormalMapTexture(shared<texture_2d> NormalMapTexture) {
            if (!NormalMapTexture) return;

            if (this->NormalMapTexture) this->NormalMapTexture->Free();
            this->NormalMapTexture = NormalMapTexture;
            if (!Data.IsNormalMap)
            {
                Data.IsNormalMap = true;
                DataBuffer->Update(&Data, sizeof(Data));
            }
        }

        /*! Default material for blin-phong lighting model constructor. */
        material_phong() = default;

        /*! Default material data defautl copy constructor. */
        material_phong(const material_phong &Other) = default;

        /*!*
         * Material for blin-phong lighting model class contructor.
         *
         * \param Diffuse  - Material diffuse lighting coefficient.
         * \param Specular - Material specular lighting coefficient.
         * \param Shininess  - Material shiness exponent lighting coefficient.
         */
        material_phong(const vec3 &Diffuse, const vec3 &Specular, float Shininess) :
            material(render_bridge::GetPhongGeometryShader())
        {
            Data.Diffuse = Diffuse;
            Data.Specular = Specular;
            Data.Shininess = Shininess;

            DataBuffer = constant_buffer::Create(&Data, sizeof(buffer_data));
        }

        /*!*
         * Bind material to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override
        {
            if (Shader != nullptr) Shader->Bind();
            if (DataBuffer != nullptr) DataBuffer->Bind(render_context::BINDING_POINT_MATERIAL_DATA);

            if (DiffuseMapTexture && Data.IsDiffuseMap) DiffuseMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_DIFFUSE);
            if (SpecularMapTexture && Data.IsSpecularMap) SpecularMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_SPECULAR);
            if (EmissionMapTexture && Data.IsEmissionMap) EmissionMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_EMISSION_MAP);
            if (NormalMapTexture && Data.IsNormalMap) NormalMapTexture->Bind(render_context::TEXTURE_SLOT_MATERIAL_NORMAL_MAP);
        }

        /*!*
         * Unbind material from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override
        {
            if (Shader != nullptr) Shader->Unbind();
            if (DataBuffer != nullptr) DataBuffer->Unbind();
            if (SpecularMapTexture && Data.IsSpecularMap) SpecularMapTexture->Unbind();
            if (DiffuseMapTexture && Data.IsDiffuseMap) DiffuseMapTexture->Unbind();
            if (NormalMapTexture && Data.IsNormalMap) NormalMapTexture->Unbind();
        }

       /*!*
        * Material for blin-phong lighting model class contructor.
        *
        * \param Specular - Material specular lighting coefficient.
        * \param Diffuse - Material diffuse lighting coefficient.
        * \param Shininess - Material shiness exponent lighting coefficient.
        */
        static shared<material_phong> Create(const vec3 &Diffuse, const vec3 &Specular, float Shininess)
        {
            return CreateShared<material_phong>(Diffuse, Specular, Shininess);
        }
    };
}
