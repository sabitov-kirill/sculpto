#version 450 core

/* Shader input  data */
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aColor;

uniform mat4 MatrWVP;

/* Shader output data. */
out vec3 DrawNormal;
out vec3 DrawPos;
out vec2 DrawTexCoords;
out vec4 DrawColor;

void main()
{
    DrawColor = aColor;
    DrawTexCoords = aTexCoords;
    DrawNormal = /* Some math */ aNormal;
    DrawPos = /* Some math */ aPos;

    gl_Position = MatrWVP * vec4(aPos, 1.0);
}