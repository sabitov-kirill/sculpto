/*****************************************************************//**
 * \file   shader.h
 * \brief  Shader program interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Shader type enum. */
    enum class shader_type
    {
        VERTEX,
        GEOMETRY,
        PIXEL,
        COMPUTE,
    };

    class shader_props
    {
    private: /* Shader data. */
        shader_type Type {};
        std::string Source {};

        std::string Path {};
        bool Watch {}; // TODO: Shader source code file reload on change

    public:
        /* Shader type getter function. */
        shader_type GetType() const { return Type; }
        /* Shader source code getter function. */
        const std::string &GetSource() const { return Source; }
        /* Shader sorce code file location getter function. */
        const std::string &GetPath() const { return Path; }
        /* Shader source code file changes watching flag getter function. */
        bool GetWatch() const { return Watch; }

        /**
         * Shader constructor by source code.
         * 
         * \param Type - shader_props type.
         * \param Source - shader_props source code.
         */
        shader_props(shader_type Type, const std::string &Source)
            : Type(Type), Source(Source), Path("Shader created from string literal"), Watch(false) {}

        /**
         * Shader constructor, loading source code from file.
         * 
         * \param Type - shader_props type.
         * \param Path - path to shader_props source code file
         * \param Watch - shader_props source code file changes watching flag
         */
        shader_props(shader_type Type, const std::string &Path, bool Watch) :
            Type(Type), Path(Path), Watch(Watch)
        {
            std::ifstream shader_file(Path, std::ios::in, std::ios::binary);
            if (!shader_file.is_open()) SCL_CORE_ERROR("Shader file \"{}\" is not found!", Path);

            std::stringstream buffer;
            buffer << shader_file.rdbuf();
            Source = buffer.str();
        }
    };

    /* Shader progream interface. */
    class shader_program abstract
    {
    protected: /* Shader program data. */
        std::vector<shader_props> Shaders;
        std::string DebugName;

    public: /* Saader program getter/setter functions. */
        /* Shader program shaders array getter function. */
        const std::vector<shader_props> &GetShareds() const { return Shaders; }
        /* Degug name getter function. */
        const std::string &GetDebugName() const { return DebugName; }

        std::vector<shader_props>::iterator begin() { return Shaders.begin(); }
        std::vector<shader_props>::iterator end() { return Shaders.end(); }
        std::vector<shader_props>::const_iterator begin() const { return Shaders.begin(); }
        std::vector<shader_props>::const_iterator end() const { return Shaders.end(); }

        /**
         * Set uniform variable to shader_props function.
         *
         * \param Name - unifrom variable name.
         * \param Value - unifrom variable name.
         * \return success flag.
         */

        virtual bool SetBool(const std::string &Name, bool Value) const = 0;
        virtual bool SetFloat(const std::string &Name, float Value) const = 0;
        virtual bool SetFloat2(const std::string &Name, const vec2 &Value) const = 0;
        virtual bool SetFloat3(const std::string &Name, const vec3 &Value) const = 0;
        virtual bool SetFloat4(const std::string &Name, const vec4 &Value) const = 0;
        virtual bool SetInt(const std::string &Name, int Value) const = 0;
        virtual bool SetInt2(const std::string &Name, const ivec2 &Value) const = 0;
        virtual bool SetInt3(const std::string &Name, const ivec3 &Value) const = 0;
        virtual bool SetInt4(const std::string &Name, const ivec4 &Value) const = 0;
        virtual bool SetUInt(const std::string &Name, u32 Value) const = 0;
        virtual bool SetUInt2(const std::string &Name, const uvec2 &Value) const = 0;
        virtual bool SetUInt3(const std::string &Name, const uvec3 &Value) const = 0;
        virtual bool SetUInt4(const std::string &Name, const uvec4 &Value) const = 0;
        virtual bool SetMatr3(const std::string &Name, const matr3 &Value) const = 0;
        virtual bool SetMatr4(const std::string &Name, const matr4 &Value) const = 0;

        /**
         * Set uniform variable to shader_props function.
         *
         * \param Location - platform specific shader variable location identifier.
         * \param Value - unifrom variable value.
         * \return success flag.
         */

        virtual bool SetBool(int Location, bool Value) const = 0;
        virtual bool SetFloat (int Location, float Value) const = 0;
        virtual bool SetFloat2(int Location, const vec2 &Value) const = 0;
        virtual bool SetFloat3(int Location, const vec3 &Value) const = 0;
        virtual bool SetFloat4(int Location, const vec4 &Value) const = 0;
        virtual bool SetInt(int Location, int Value) const = 0;
        virtual bool SetInt2(int Location, const ivec2 &Value) const = 0;
        virtual bool SetInt3(int Location, const ivec3 &Value) const = 0;
        virtual bool SetInt4(int Location, const ivec4 &Value) const = 0;
        virtual bool SetUInt(int Location, u32 Value) const = 0;
        virtual bool SetUInt2(int Location, const uvec2 &Value) const = 0;
        virtual bool SetUInt3(int Location, const uvec3 &Value) const = 0;
        virtual bool SetUInt4(int Location, const uvec4 &Value) const = 0;
        virtual bool SetMatr3(int Location, const matr3 &Value) const = 0;
        virtual bool SetMatr4(int Location, const matr4 &Value) const = 0;

    public:
        /**
         * Shader program default constructor.
         * 
         * \param Shaders - shaders array.
         * \param DubugName - shader program debug name.
         */
        shader_program(const std::initializer_list<shader_props> &Shaders, const std::string &DebugName);

        /**
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const = 0;

        /**
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /**
         * Update (recompile from dource file).
         * 
         * \param None.
         * \return None.
         */
        virtual void Update() = 0;

        /**
         * Unload shader program from GPU memory function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /**
         * Shader program creation function.
         * 
         * \param Shaders - shaders array.
         * \param DubugName - shader program debug name.
         * \return Pointer to created shader_props program.
         */
        static shared<shader_program> Create(const std::initializer_list<shader_props> &Shaders, const std::string &DebugName);

        /**
         * Shader program creation function, loading all shaders from directory.
         * Shader files formath: vertex.(glsl/hlsl)
         *                       geometry.(glsl/hlsl)
         *                       pixel.(glsl/hlsl)
         *                       compute.(glsl/hlsl)
         *
         * \param Shaders - shaders array.
         * \return Pointer to created shader_props program.
         */
        static shared<shader_program> Create(const std::string &FolderPath);
    };
}
