/*****************************************************************//**
 * \file   gl_shader.h
 * \brief  OpenGL shader program class definition module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#pragma once

#include "core/render/primitives/shader.h"
#include "gl.h"

namespace scl
{
    /* OpenGL shader_props program class. */
    class gl_shader_program : public shader_program
    {
    private: /* Shader program data. */
        GLuint Id {};
        static int CurrentlyBoundShaderId;
        mutable std::unordered_map<std::string, int> VariablesLocations {};

    private:
        /**
         * Get OpenGL shader type function.
         *
         * \param Type - shader type.
         * \return OpenGL shader type.
         */
        constexpr static GLenum GetGLShaderType(shader_type Type);

        /**
         * Compile shader_props from source code function.
         * 
         * \param Source - shader_props source
         * \param ShaderId shader_props OpenGL id.
         * \return success flag.
         */
        bool CompileShader(const shader_props &Shader, GLuint &ShaderId);

        /**
         * Compile all shaders and create shader_props program function.
         * 
         * \param Shaders - array of shaders id's to attach to shader_props program.
         * \return success flag.
         */
        bool CompileProgram(const std::vector<GLuint> &Shaders);

        /**
         * Create shader_props program from shaders data arary, stored in base shader_props class.
         * 
         * \param None.
         * \return success flag.
         */
        bool Create();

        /**
         * Get shader variable location from cache if ys exist or create it ande cache to memory function.
         * 
         * \param Name - name of shader variable.
         * \return shader variable location.
         */
        int GetOrCacheLocation(const std::string &Name) const;

    public:
        /**
         * Shader program default constructor.
         *
         * \param Shaders - shaders array.
         * \param DubugName - shader program debug name.
         */
        gl_shader_program(const std::initializer_list<shader_props> &Shaders, const std::string &DebugName);

        /* Default destructor. */
        ~gl_shader_program();

        /**
         * Set uniform variable to shader_props function.
         *
         * \param Name - unifrom variable name.
         * \param Value - unifrom variable name.
         * \return success flag.
         */

        bool SetBool(const std::string &Name, bool Value) const override;
        bool SetFloat(const std::string &Name, float Value) const override;
        bool SetFloat2(const std::string &Name, const vec2 &Value) const override;
        bool SetFloat3(const std::string &Name, const vec3 &Value) const override;
        bool SetFloat4(const std::string &Name, const vec4 &Value) const override;
        bool SetInt(const std::string &Name, int Value) const override;
        bool SetInt2(const std::string &Name, const ivec2 &Value) const override;
        bool SetInt3(const std::string &Name, const ivec3 &Value) const override;
        bool SetInt4(const std::string &Name, const ivec4 &Value) const override;
        bool SetUInt(const std::string &Name, u32 Value) const override;
        bool SetUInt2(const std::string &Name, const uvec2 &Value) const override;
        bool SetUInt3(const std::string &Name, const uvec3 &Value) const override;
        bool SetUInt4(const std::string &Name, const uvec4 &Value) const override;
        bool SetMatr3(const std::string &Name, const matr3 &Value) const override;
        bool SetMatr4(const std::string &Name, const matr4 &Value) const override;

        bool SetBool(int Location, bool Value) const override;
        bool SetFloat(int Location, float Value) const;
        bool SetFloat2(int Location, const vec2 &Value) const;
        bool SetFloat3(int Location, const vec3 &Value) const;
        bool SetFloat4(int Location, const vec4 &Value) const;
        bool SetInt(int Location, int Value) const;
        bool SetInt2(int Location, const ivec2 &Value) const;
        bool SetInt3(int Location, const ivec3 &Value) const;
        bool SetInt4(int Location, const ivec4 &Value) const;
        bool SetUInt(int Location, u32 Value) const;
        bool SetUInt2(int Location, const uvec2 &Value) const;
        bool SetUInt3(int Location, const uvec3 &Value) const;
        bool SetUInt4(int Location, const uvec4 &Value) const;
        bool SetMatr3(int Location, const matr3 &Value) const;
        bool SetMatr4(int Location, const matr4 &Value) const;

        /**
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Bind() const override;

        /**
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        void Unbind() const override;

        /**
         * Update (recompile from dource file).
         *
         */
        void Update() override;

        /**
         * Unload shader_props program from GPU memory function.
         *
         * \param None.
         * \return None.
         */
        void Free() override;
    };
}
