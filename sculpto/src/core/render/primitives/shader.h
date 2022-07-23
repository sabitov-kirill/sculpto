/*!****************************************************************//*!*
 * \file   shader.h
 * \brief  Shader program interface definition module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#pragma once

#include "render_primitive.h"

namespace scl
{
    /*! Shader type enum. */
    enum class shader_type
    {
        VERTEX,
        GEOMETRY,
        PIXEL,
        COMPUTE,
    };

    /*! Shader creation properties structure. */
    struct shader_props
    {
        shader_type Type {};
        std::string Source {};

        /*! Shader properties default constructor. */
        shader_props() = default;

        /*!*
         * Shader constructor by source code.
         * 
         * \param Type - shader_props type.
         * \param Source - shader_props source code.
         */
        shader_props(shader_type Type, const std::string &Source) : Type(Type), Source(Source) {}
    };

    /*! Shader progream interface. */
    class shader_program: public render_primitive
    {
    public: /*! Shader program data. */
        std::string SingleSourceFileName {};
        std::string VertexShadersourceFileName {};
        std::string GeometryShadersourceFileName {};
        std::string PixelShadersourceFileName {};
        std::string DebugName {};

    public: /*! Saader program getter/setter functions. */
        /*!*
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

        /*!*
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
        /*! Shader program default constructor. */
        shader_program(const std::string &DebugName);

        /*! Shader program default deatructor. */
        virtual ~shader_program() = default;

        /*!*
         * Bind buffer to current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Bind() const = 0;

        /*!*
         * Unbind buffer from current render stage function.
         *
         * \param None.
         * \return None.
         */
        virtual void Unbind() const = 0;

        /*!*
         * Recompile shader program function.
         * 
         * \param Shaders - shaders array.
         * \return None.
         */
        virtual void Update(const std::vector<shader_props> &Shaders) = 0;

        /*!*
         * Unload shader program from GPU memory function.
         * 
         * \param None.
         * \return None.
         */
        virtual void Free() = 0;

        /*!*
         * Shader program creation function.
         * 
         * \param Shaders - shaders array.
         * \param DubugName - shader program debug name.
         * \return Pointer to created shader_props program.
         */
        static shared<shader_program> Create(const std::vector<shader_props> &Shaders, const std::string &DebugName);
    };
}
