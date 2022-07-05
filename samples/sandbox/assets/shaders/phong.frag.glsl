#version 460

#define BINDING_POINT_MESH_DATA           0
#define BINDING_POINT_MATERIAL_DATA       5
#define BINDING_POINT_FREE                20
#define TEXTURE_SLOT_MATERIAL_DIFFUSE     0
#define TEXTURE_SLOT_MATERIAL_SPECULAR    1
#define TEXTURE_SLOT_MATERIAL_NORMAL_MAP  2

struct material
{
    vec3 Specular;
    bool IsSpecularMap;
    vec3 Diffuse;
    bool IsDiffuseMap;
    float Shininess;
    bool IsEmissionMap;
    bool IsNormalMap;
};

struct light
{
    vec3 Position;
    vec3 Color;
};

/* Shader input  data */
layout(location = BINDING_POINT_MATERIAL_DATA)      uniform material u_Material;
layout(binding  = TEXTURE_SLOT_MATERIAL_DIFFUSE)    uniform sampler2D u_DiffuseMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_SPECULAR)   uniform sampler2D u_SpecularMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_EMISSION)   uniform sampler2D u_EmissionMap;
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

vec3 Shade(light Light)
{
    vec3 norm = normalize(o_Normal);

    // ambient
    vec3 ambient = Light.Color * u_EnviromentAmbient;

    // diffuse
    vec3 light_dir = normalize(Light.Position - o_Pos);
    float diffuse_strength = max(dot(norm, light_dir), 0.0);
    vec3 diffuse_coeff = vec3(0, 0, 0);
    if (u_Material.IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
    else                         diffuse_coeff = u_Material.Diffuse;
    vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(u_CameraPosition - o_Pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if (u_Material.IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
    else                          spec_coeff = u_Material.Specular;
    vec3 specular = Light.Color * (spec_strength * spec_coeff);

    return ambient + diffuse + specular;
}

void main()
{
    light L = light(vec3(2, 2, 2), vec3(1, 1, 1));
    OutColor = vec4(Shade(L), 1);
}