/*****************************************************************//**
 * \file   gl_shader.cpp
 * \brief  OpenGL shader program class implementatino module.
 * 
 * \author Sabitov Kirill
 * \date   27 June 2022
 *********************************************************************/

#include "sclpch.h"
#include "gl_shader.h"

int scl::gl_shader_program::CurrentlyBoundShaderId {};

constexpr GLenum scl::gl_shader_program::GetGLShaderType(shader_type Type)
{
    switch (Type)
    {
    case shader_type::VERTEX:    return GL_VERTEX_SHADER;
    case shader_type::GEOMETRY:  return GL_GEOMETRY_SHADER;
    case shader_type::PIXEL:     return GL_FRAGMENT_SHADER;
    case shader_type::COMPUTE:   return GL_COMPUTE_SHADER;
    }

    SCL_CORE_ASSERT(0, "Unknown shader type.");
    return GLenum();
}

bool scl::gl_shader_program::CompileShader(const shader_props &Shader, GLuint &ShaderId)
{
    if (ShaderId != 0) glDeleteShader(ShaderId);
    SCL_CORE_ASSERT((ShaderId = glCreateShader(GetGLShaderType(Shader.GetType()))) != 0,
                    "Error in creation OpenGL shader primitive.");

    int res {};
    // Temporary object creation to prevent dangling pointer
    std::string source_str = Shader.GetSource();
    const char *source = source_str.c_str();
    char ErrorLog[1000] {};
    glShaderSource(ShaderId, 1, &source, nullptr);
    glCompileShader(ShaderId);
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &res);
    if (!res)
    {
        glGetShaderInfoLog(ShaderId, sizeof(ErrorLog), &res, ErrorLog);
        SCL_CORE_WARN("Error in compiling shader \"{}\" of type {} during shader program \"{}\" creation.\nError log:\n{}",
                      Shader.GetPath(), (int)Shader.GetType(), DebugName, std::string(ErrorLog));
        return false;
    }

    return true;
}

bool scl::gl_shader_program::CompileProgram(const std::vector<GLuint> &Shaders)
{
    this->Free();
    SCL_CORE_ASSERT((Id = glCreateProgram()) != 0, "Error in creation OpenGL shader program premitive.");

    int res {};
    char ErrorLog[1000] {};
    for (GLuint shader_id : Shaders) glAttachShader(Id, shader_id);
    glLinkProgram(Id);
    glGetProgramiv(Id, GL_LINK_STATUS, &res);
    if (!res)
    {
        glGetProgramInfoLog(Id, sizeof(ErrorLog), &res, ErrorLog);
        SCL_CORE_WARN("Error in linking shader program \"{}\".\nError log:\n{}", DebugName, std::string(ErrorLog));
        return false;
    }
    return true;
}

bool scl::gl_shader_program::Create()
{
    u32 shaders_count = (u32)Shaders.size();

    // Compile all passed shaders
    std::vector<GLuint> shaders_ids(shaders_count);
    for (u32 i = 0; i < shaders_count; i++)
        if (!CompileShader(Shaders[i], shaders_ids[i])) {
            // Delete all created shaders
            for (auto shader_id : shaders_ids)
                if (shader_id != 0) glDeleteShader(shader_id);
            return false;
        }

    // Compile shader program
    if (!CompileProgram(shaders_ids))
    {
        // Deatach and delete all created shaders
        for (auto shader_id : shaders_ids) {
            if (shader_id != 0) {
                if (Id != 0) glDetachShader(Id, shader_id);
                glDeleteShader(shader_id);
            }
        }
        if (Id != 0) glDeleteProgram(Id), Id = 0;
        return false;
    }

    return true;
}

int scl::gl_shader_program::GetOrCacheLocation(const std::string &Name) const
{
    auto it = VariablesLocations.find(Name);
    if (it != VariablesLocations.end()) return it->second;

    int location = glGetUniformLocation(Id, Name.c_str());
    VariablesLocations.emplace(Name, location);
    if (location == -1) 
        SCL_CORE_WARN("Currently bound shader \"{}\" don't have active variable with name \"{}\".",
                        DebugName, Name);
    return location;
}

scl::gl_shader_program::gl_shader_program(const std::initializer_list<shader_props> &Shaders, const std::string &DebugName) :
    shader_program(Shaders, DebugName)
{
    Create();
}

scl::gl_shader_program::~gl_shader_program()
{
    Free();
}

void scl::gl_shader_program::Bind() const
{
    if (glIsProgram(Id))
    {
        glUseProgram(Id);
        gl_shader_program::CurrentlyBoundShaderId = Id;
    }
}

void scl::gl_shader_program::Unbind() const
{
    glUseProgram(0);
    CurrentlyBoundShaderId = 0;
}

void scl::gl_shader_program::Update()
{
    Free();
    Create();
}

void scl::gl_shader_program::Free()
{
    VariablesLocations.clear();

    if (Id == 0) return;

    int shaders_count {};
    GLuint shaders_ids[5] {};
    glGetAttachedShaders(Id, 5, &shaders_count, shaders_ids);

    for (int i = 0; i < shaders_count; i++)
    {
        glDetachShader(Id, shaders_ids[i]);
        glDeleteShader(shaders_ids[i]);
    }
    glDeleteProgram(Id), Id = 0;
}

bool scl::gl_shader_program::SetFloat(const std::string &Name, float Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform1f(location, Value);
    return true;
}

bool scl::gl_shader_program::SetFloat2(const std::string &Name, const vec2 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform2fv(location, 1, (float *)&Value);
    return true;
}

bool scl::gl_shader_program::SetFloat3(const std::string &Name, const vec3 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform3fv(location, 1, (float *)&Value);
    return true;
}

bool scl::gl_shader_program::SetFloat4(const std::string &Name, const vec4 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform4fv(location, 1, (float *)&Value);
    return true;
}

bool scl::gl_shader_program::SetInt(const std::string &Name, int Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform1i(location, Value);
    return true;
}

bool scl::gl_shader_program::SetInt2(const std::string &Name, const ivec2 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform2iv(location, 1, (int *)&Value);
    return true;
}

bool scl::gl_shader_program::SetInt3(const std::string &Name, const ivec3 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform3iv(location, 1, (int *)&Value);
    return true;
}

bool scl::gl_shader_program::SetInt4(const std::string &Name, const ivec4 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform4iv(location, 1, (int *)&Value);
    return true;
}

bool scl::gl_shader_program::SetUInt(const std::string &Name, u32 Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform1ui(location, Value);
    return true;
}

bool scl::gl_shader_program::SetUInt2(const std::string &Name, const uvec2 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform2uiv(location, 1, (u32 *)&Value);
    return true;
}

bool scl::gl_shader_program::SetUInt3(const std::string &Name, const uvec3 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform3uiv(location, 1, (u32 *)&Value);
    return true;
}

bool scl::gl_shader_program::SetUInt4(const std::string &Name, const uvec4 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);

    glUniform4uiv(location, 1, (u32 *)&Value);
    return true;
}

bool scl::gl_shader_program::SetMatr4(const std::string &Name, const matr4 &Value) const
{
    SCL_CORE_ASSERT(Id == gl_shader_program::CurrentlyBoundShaderId,
                    "Trying set shader variable to not binded shader.");
    int location = GetOrCacheLocation(Name);
    if (location == -1) return false;

    glUniformMatrix4fv(location, 1, FALSE, (float *)&Value);
    return true;
}
