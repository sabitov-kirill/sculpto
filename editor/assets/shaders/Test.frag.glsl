#version 450 core

/* Shader input  data */
layout(binding = 0) uniform sampler2D aTex;

uniform  float  Time;
in       vec3   DrawNormal;
in       vec3   DrawPos;
in       vec2   DrawTexCoords;
in       vec4   DrawColor;

/* Shader output data. */
out vec4  FragColor;

void main()
{
    // FragColor = vec4(sin(Time), 0.3f, cos(Time), 1.0f);
    // FragColor = DrawColor;
    FragColor = texture(aTex, DrawTexCoords);
    // FragColor = vec4(DrawTexCoords, 0, 1);
    // FragColor = vec4(DrawNormal, 1);
}