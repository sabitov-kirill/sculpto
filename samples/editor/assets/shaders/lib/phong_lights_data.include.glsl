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
    bool IsShadows;
    vec3 Color;
    float dummy2;
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
    point_light        u_PointLights[50];
    directional_light  u_DirectionalLight;
    spot_light         u_SpotLights[50];
    uint               u_PointLightsCount;
    bool               u_IsDirectionalLight;
    uint               u_SpotLightsCount;
};

layout(binding  = TEXTURE_SLOT_SHADOW_MAP) uniform sampler2D u_ShadowMap;