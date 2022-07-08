#version 460

#define BINDING_POINT_SCENE_DATA               0
#define BINDING_POINT_MATERIAL_DATA            5
#define BINDING_POINT_LIGHTS_STORAGE           10
#define BINDING_POINT_SHADOW_CASTERS_STORAGE   11
#define BINDING_POINT_FREE                     20
#define TEXTURE_SLOT_MATERIAL_DIFFUSE          0
#define TEXTURE_SLOT_MATERIAL_SPECULAR         1
#define TEXTURE_SLOT_MATERIAL_NORMAL_MAP       2
#define LIGHTS_MAX_POINT                       50
#define LIGHTS_MAX_SPOT                        50

/* Shader input  data */
layout (location = 0) in vec3 v_Pos;

uniform mat4 u_MatrWVP;

void main()
{
    gl_Position = u_MatrWVP * vec4(v_Pos, 1.0);
}