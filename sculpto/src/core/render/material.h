/*****************************************************************//**
 * \file   material.h
 * \brief  Mesh material class deinition module.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "primitives/shader.h"
#include "primitives/texture.h"
#include "primitives/buffer.h"

namespace scl
{
    /* Material data classes declaration. */

    /* Mesh material class. */
    class material
    {
    private: /* Material data. */
        shared<shader_program> Shader {};

        shared<constant_buffer> DataBuffer {};
        void *Data {};
        size_t DataSize {};

        shared<texture_2d> SurfaceTexture {};
     /* shared<texture_2d> NormalMapTexture; */
     /* shared<texture_2d> AlbedoMapTexture; etc. */

    public: /* Material data getter/setter functions. */
        /* Material shdaer getter function. */
        const shared<shader_program> &GetShader() const;
        /* Material surface texture getter function. */
        const shared<texture_2d> &GetSurfaceTexture() const;
        /* Material shader getter funcrion. */
        void SetShader(shared<shader_program> Shader);
        /* Material surface texture setter function. */
        void SetSurfaceTexture(shared<texture_2d> SurfaceTexture);
        /* Material data setter function. */
        void SetMeterialData(void *Data, size_t DataSize);

    public:
        /**
         * Material constructor by shader only.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         */
        material(shared<shader_program> Shader);

        /**
         * Material constructor by shader and material data.
         * 
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         */
        material(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize);

        /**
         * Material constructor by shader, material data and surface texture.
         * 
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         * \param SurfaceTexture - material surface texture.
         */
        material(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize, shared<texture_2d> SurfaceTexture);

        /**
         * Material constructor by shader, material data and image for surface texture creation.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         * \param SurfaceTextureImage - image for creating material surface texture.
         */
        material(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize, const image &SurfaceTextureImage);

        /**
         * Material constructor by shader, material data and image file for surface texture creation.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         * \param SurfaceTextureFileName - image file name for material surface texture creation.
         */
        material(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize, const std::string &SurfaceTextureFileName);

    public:
        /**
         * Bind material to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const;

        /**
         * Unbind material from current render stage function.
         *
         * \param Slot - texture slot to unbind it from.
         * \return None.
         */
        void Unbind() const;

        /**
         * Material creation function by shader only.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         */
        static shared<material> Create(shared<shader_program> Shader)
        {
            return CreateShared<material>(Shader);
        }

        /**
         * Material creation function by shader and material data.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         */
        static shared<material> Create(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize)
        {
            return CreateShared<material>(Shader, MaterialData, MaterialDataSize);
        }

        /**
         * Material creation function by shader, material data and surface texture.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         * \param SurfaceTexture - material surface texture.
         */
        static shared<material> Create(shared<shader_program> Shader, void *MaterialData,
                                       size_t MaterialDataSize, shared<texture_2d> SurfaceTexture)
        {
            return CreateShared<material>(Shader, MaterialData, MaterialDataSize, SurfaceTexture);
        }

        /**
         * Material creation function by shader, material data and image for surface texture creation.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         * \param SurfaceTextureImage - image for creating material surface texture.
         */
        static shared<material> Create(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize,
                                       const image &SurfaceTextureImage)
        {
            return CreateShared<material>(Shader, MaterialData, MaterialDataSize, SurfaceTextureImage);
        }

        /**
         * Material creation function by shader, material data and image file for surface texture creation.
         *
         * \param Shader - material shader to use while mesh with this material rendering.
         * \param MaterialData - material data (surface coeffs e.g.) structure pointer.
         * \param MaterialDataSize - material data (surface coeffs e.g.) structure size.
         * \param SurfaceTextureFileName - image file name for material surface texture creation.
         */
        static shared<material> Create(shared<shader_program> Shader, void *MaterialData, size_t MaterialDataSize,
                                       const std::string &SurfaceTextureFileName)
        {
            return CreateShared<material>(Shader, MaterialData, MaterialDataSize, SurfaceTextureFileName);
        }
    };
}
