#version 460

#define BINDING_POINT_SCENE_DATA               0
#define BINDING_POINT_MATERIAL_DATA            5
#define BINDING_POINT_LIGHTS_STORAGE           10
#define BINDING_POINT_SHADOW_CASTERS_STORAGE   11
#define BINDING_POINT_FREE                     20
#define TEXTURE_SLOT_MATERIAL_DIFFUSE          0
#define TEXTURE_SLOT_MATERIAL_SPECULAR         1
#define TEXTURE_SLOT_MATERIAL_NORMAL_MAP       2
#define TEXTURE_SLOT_SHADOW_MAP                5
#define LIGHTS_MAX_POINT                       50
#define LIGHTS_MAX_SPOT                        50

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

/* Current pipieline data. */
layout(std140, binding = BINDING_POINT_SCENE_DATA) uniform ubo_PipelineData
{
    vec3   CameraDirection;     /* Submission camera direction vector. */
    float  Time;                /* Currently rendering frame time since porgram time. */
    vec3   CameraPosition;      /* Submission camer location vector. */
    int    ViewportWidth;       /* Currently rendering frame viewport width. */
    int    ViewportHeight;      /* Currently rendering frame viewport height. */
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

/* Currently rendering mesh material data. */
layout(std140, binding = BINDING_POINT_MATERIAL_DATA) uniform ubo_Material
{
    vec3 Specular;
    float Shininess;
    vec3 Diffuse;
    int IsSpecularMap;
    int IsDiffuseMap;
    int IsNormalMap;
};

layout(binding  = TEXTURE_SLOT_MATERIAL_DIFFUSE)    uniform sampler2D u_DiffuseMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_SPECULAR)   uniform sampler2D u_SpecularMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_NORMAL_MAP) uniform sampler2D u_NormalMap;
layout(binding  = TEXTURE_SLOT_SHADOW_MAP)          uniform sampler2D u_ShadowMap;

vec3 u_EnviromentAmbient = vec3(0.03, 0.028, 0.005);

in VS_OUT
{
    vec3 Normal;
    vec3 Pos;
    vec2 TexCoords;
    vec4 Color;
    vec4 LightSpacePos;
} fs_in;

/* Shader output data. */
layout(location = 0) out vec4 OutColor;

float ShadowFactorCalculate(vec4 ShadowLightSpacePos)
{
    vec3 proj_coords = ShadowLightSpacePos.xyz / ShadowLightSpacePos.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    float closest_depth = texture(u_ShadowMap, proj_coords.xy).r;
    float current_depth = proj_coords.z;

    float bias = 0.005;
    return current_depth - bias > closest_depth ? 1.0 : 0.0;
}

vec3 PointLightShade(point_light Light, vec3 Normal)
{
    float dist = length(Light.Position - fs_in.Pos);
    float attenuation = 1.0 / (Light.Constant + Light.Linear * dist + Light.Quadratic * (dist * dist));

    // ambient
    vec3 ambient =  Diffuse * u_EnviromentAmbient;

    // diffuse
    vec3 light_dir = normalize(Light.Position - fs_in.Pos);
    float diffuse_strength = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse_coeff = vec3(0, 0, 0);
    if ( IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, fs_in.TexCoords).rgb;
    else               diffuse_coeff =  Diffuse;
    vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(CameraPosition - fs_in.Pos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0),  Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if ( IsSpecularMap) spec_coeff = texture(u_SpecularMap, fs_in.TexCoords).rgb;
    else                spec_coeff =  Specular;
    vec3 specular = Light.Color * (spec_strength * spec_coeff);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 DirectionalLightShade(directional_light Light, vec3 Normal)
{
    // ambient
    vec3 ambient = u_EnviromentAmbient;

    // diffuse
    vec3 light_dir = normalize(-Light.Direction);
    float diffuse_strength = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse_coeff = vec3(0, 0, 0);
    if ( IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, fs_in.TexCoords).rgb;
    else               diffuse_coeff = Diffuse;
    vec3 diffuse = (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(CameraPosition - fs_in.Pos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0),  Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if ( IsSpecularMap) spec_coeff = texture(u_SpecularMap, fs_in.TexCoords).rgb;
    else                spec_coeff =  Specular;
    vec3 specular = (spec_strength * spec_coeff);

    // shadows
    if (Light.IsShadows)
    {
        float shadow = ShadowFactorCalculate(fs_in.LightSpacePos);
        return (ambient + (1 - shadow) * (diffuse + specular)) * Light.Color;
    }
    return (ambient + (diffuse + specular)) * Light.Color;
}

vec3 SpotLightShade(spot_light Light, vec3 Normal)
{
    vec3 light_dir = normalize(Light.Position - fs_in.Pos);
    float theta = dot(light_dir, normalize(-Light.Direction));
    float intensity = clamp((theta - Light.OuterCutoffCos) / Light.Epsilon, 0.0, 1.0);

    if(intensity >= 0) 
    {
        // ambient
        vec3 ambient = Light.Color * u_EnviromentAmbient;

        // diffuse
        vec3 light_dir = normalize(Light.Position - fs_in.Pos);
        float diffuse_strength = max(dot(Normal, light_dir), 0.0);
        vec3 diffuse_coeff = vec3(0, 0, 0);
        if ( IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, fs_in.TexCoords).rgb;
        else               diffuse_coeff = Diffuse;
        vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

        // specular
        vec3 view_dir = normalize(CameraPosition - fs_in.Pos);
        vec3 reflect_dir = reflect(-light_dir, Normal);
        float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0),  Shininess);
        vec3 spec_coeff = vec3(0, 0, 0);
        if (IsSpecularMap) spec_coeff = texture(u_SpecularMap, fs_in.TexCoords).rgb;
        else               spec_coeff =  Specular;
        vec3 specular = Light.Color * (spec_strength * spec_coeff);

        return ambient + (diffuse + specular) * intensity;
    }
    else
      if ( IsDiffuseMap) return u_EnviromentAmbient * texture(u_DiffuseMap, fs_in.TexCoords).xyz;
      else               return u_EnviromentAmbient *  Diffuse;
}

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 color = vec3(0, 0, 0);

    for (uint i = 0; i < PointLightsCount; ++i) color += PointLightShade(PointLights[i], norm);
    for (uint i = 0; i < SpotLightsCount; ++i)  color += SpotLightShade(SpotLights[i], norm);
    if (IsDirectionalLight)                     color += DirectionalLightShade(DirectionalLight, norm);

     OutColor = vec4(color, 1);
}
