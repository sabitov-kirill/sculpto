#version 460

/* Shader input  data */
layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoords;
layout (location = 3) in vec4 v_Color;

uniform mat3 u_MatrN;
uniform mat4 u_MatrW;
uniform mat4 u_MatrWVP;

/* Shader output data. */
out vec3 o_Normal;
out vec3 o_Pos;
out vec2 o_TexCoords;
out vec4 o_Color;

void main()
{
    o_Color     = v_Color;
    o_TexCoords = v_TexCoords;
    o_Normal    = u_MatrN * v_Normal;
    o_Pos       = vec3(u_MatrW * vec4(v_Pos, 1.0));

    gl_Position = u_MatrWVP * vec4(v_Pos, 1.0);
}