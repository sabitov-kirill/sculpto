#version 460

/* Shader input  data */
layout (location = 0) in vec3 v_Pos;
layout (location = 3) in vec4 v_Color;

uniform mat4 u_MatrWVP;

/* Shader output data. */
out vec4 o_Color;

void main()
{
    o_Color      = v_Color;
    gl_Position  = u_MatrWVP * vec4(v_Pos, 1.0);
}