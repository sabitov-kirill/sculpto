#version 460

/* Shader inpur data. */
in vec4 o_Color;

/* Shader output data. */
layout(location = 0) out vec4 OutColor;

void main()
{
    OutColor = o_Color;
}