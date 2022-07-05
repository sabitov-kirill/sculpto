#version 460

#define BINDING_POINT_MESH_DATA          0
#define BINDING_POINT_MATERIAL_DATA      5
#define BINDING_POINT_LIGHTS_STORAGE     10
#define BINDING_POINT_FREE               20
#define TEXTURE_SLOT_MATERIAL_DIFFUSE    0
#define TEXTURE_SLOT_MATERIAL_SPECULAR   1
#define TEXTURE_SLOT_MATERIAL_NORMAL_MAP 2
#define LIGHTS_MAX_POINT                 50
#define LIGHTS_MAX_SPOT                  50

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
    float __dummy;
    vec3 Color;
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

/* Shader input data. */
layout(std140, binding = BINDING_POINT_LIGHTS_STORAGE) uniform ubo_LightsStorage
{
    point_light PointLights[50];
    directional_light DirectionalLight;
    spot_light SpotLights[50];
    uint PointLightsCount;
    uint IsDirectionalLight;
    uint SpotLightsCount;
};
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

         vec3   u_EnviromentAmbient = vec3(0.03, 0.028, 0.005);
uniform  float  u_Time;
uniform  vec3   u_CameraPosition;
in       vec3   o_Normal;
in       vec3   o_Pos;
in       vec2   o_TexCoords;
in       vec4   o_Color;

/* Shader output data. */
layout(location = 0) out vec4 OutColor;

vec3 PointLightShade(point_light Light, vec3 Normal)
{
    float dist = length(Light.Position - o_Pos);
    float attenuation = 1.0 / (Light.Constant + Light.Linear * dist + Light.Quadratic * (dist * dist));

    // ambient
    vec3 ambient =  Diffuse * u_EnviromentAmbient;

    // diffuse
    vec3 light_dir = normalize(Light.Position - o_Pos);
    float diffuse_strength = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse_coeff = vec3(0, 0, 0);
    if ( IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
    else               diffuse_coeff =  Diffuse;
    vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(u_CameraPosition - o_Pos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0),  Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if ( IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
    else                spec_coeff =  Specular;
    vec3 specular = Light.Color * (spec_strength * spec_coeff);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 DirectionalLightShade(directional_light Light, vec3 Normal)
{

    // ambient
    vec3 ambient = Light.Color * u_EnviromentAmbient;

    // diffuse
    vec3 light_dir = normalize(-Light.Direction);
    float diffuse_strength = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse_coeff = vec3(0, 0, 0);
    if ( IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
    else               diffuse_coeff = Diffuse;
    vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(u_CameraPosition - o_Pos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0),  Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if ( IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
    else                spec_coeff =  Specular;
    vec3 specular = Light.Color * (spec_strength * spec_coeff);

    return ambient + diffuse + specular;
}

vec3 SpotLightShade(spot_light Light, vec3 Normal)
{
    vec3 light_dir = normalize(Light.Position - o_Pos);
    float theta = dot(light_dir, normalize(-Light.Direction));
    float intensity = clamp((theta - Light.OuterCutoffCos) / Light.Epsilon, 0.0, 1.0);

    if(intensity >= 0) 
    {
        // ambient
        vec3 ambient = Light.Color * u_EnviromentAmbient;

        // diffuse
        vec3 light_dir = normalize(Light.Position - o_Pos);
        float diffuse_strength = max(dot(Normal, light_dir), 0.0);
        vec3 diffuse_coeff = vec3(0, 0, 0);
        if ( IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
        else               diffuse_coeff = Diffuse;
        vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

        // specular
        vec3 view_dir = normalize(u_CameraPosition - o_Pos);
        vec3 reflect_dir = reflect(-light_dir, Normal);
        float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0),  Shininess);
        vec3 spec_coeff = vec3(0, 0, 0);
        if ( IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
        else                          spec_coeff =  Specular;
        vec3 specular = Light.Color * (spec_strength * spec_coeff);

        return ambient + (diffuse + specular) * intensity;
    }
    else
      if ( IsDiffuseMap) return u_EnviromentAmbient * texture(u_DiffuseMap, o_TexCoords).xyz;
      else               return u_EnviromentAmbient *  Diffuse;
}

void main()
{
    vec3 norm = normalize(o_Normal);
    vec3 color = vec3(0, 0, 0);

    for (uint i = 0; i < PointLightsCount; ++i)
        color += PointLightShade(PointLights[i], norm);
    for (uint i = 0; i < SpotLightsCount; ++i)
        color += SpotLightShade(SpotLights[i], norm);
    if (IsDirectionalLight)
        color += DirectionalLightShade(DirectionalLight, norm);

    OutColor = vec4(color, 1);
}