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

/* Scene rendering data. */
layout(std140, binding = BINDING_POINT_SCENE_DATA) uniform ubo_Scene
{
    vec3   CameraDirection;     /* Submission camera direction vector. */
    float  Time;                /* Currently rendering frame time since porgram time. */
    vec3   CameraPosition;      /* Submission camer location vector. */
    int    ViewportWidth;       /* Currently rendering frame viewport width. */
    int    ViewportHeight;      /* Currently rendering frame viewport height. */
};

/* Point light structure. */
struct point_light
{
    vec3 Position;
    float Constant;
    vec3 Color;
    float Linear;
    float Quadratic;
};

/* Directional light structure. */
struct directional_light
{
    vec3 Direction;
    uint IsShadows;
    vec3 Color;
    float __dummy2;
    mat4 ViewProjection;
};

/* Spot light structure. */
struct spot_light
{
    vec3 Position;
    float InnerCutoffCos;
    vec3 Direction;
    float OuterCutoffCos;
    vec3 Color;
    float Epsilon;
};

/* Current pipieline lights storage. */
layout(std140, binding = BINDING_POINT_LIGHTS_STORAGE) uniform ubo_LightsStorage
{
    point_light PointLights[50];
    directional_light DirectionalLight;
    spot_light SpotLights[50];
    uint PointLightsCount;
    uint IsDirectionalLight;
    uint SpotLightsCount;
};

/* Shader input  data */
layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoords;
layout (location = 3) in vec4 v_Color;

uniform mat3 u_MatrN;
uniform mat4 u_MatrW;
uniform mat4 u_MatrWVP;

/* Shader output data. */
out VS_OUT
{
    vec3 Normal;
    vec3 Pos;
    vec2 TexCoords;
    vec4 Color;
    vec4 LightSpacePos;
} vs_out;

void main()
{
    vs_out.Color         = v_Color;
    vs_out.TexCoords     = v_TexCoords;
    vs_out.Normal        = u_MatrN * v_Normal;
    vs_out.Pos           = vec3(u_MatrW * vec4(v_Pos, 1.0));

    vs_out.LightSpacePos = DirectionalLight.ViewProjection * vec4(vs_out.Pos, 1.0);

    gl_Position = u_MatrWVP * vec4(v_Pos, 1.0);
}