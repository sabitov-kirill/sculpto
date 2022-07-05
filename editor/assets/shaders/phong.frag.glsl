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
    bool IsNormalMap;
};

struct point_light
{
    vec3 Position;
    vec3 Color;

    float Constant;
    float Linear;
    float Quadratic;
};

struct directional_light
{
    vec3 Direction;
    vec3 Color;
};

struct spot_light
{
    vec3 Position;
    vec3 Direction;
    vec3 Color;

    float InnerCutoff;
    float OuterCutoff;
    float Epsilon;
};

/* Shader input  data */
layout(location = BINDING_POINT_MATERIAL_DATA)      uniform material u_Material;
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
    vec3 ambient = u_Material.Diffuse * u_EnviromentAmbient;

    // diffuse
    vec3 light_dir = normalize(Light.Position - o_Pos);
    float diffuse_strength = max(dot(Normal, light_dir), 0.0);
    vec3 diffuse_coeff = vec3(0, 0, 0);
    if (u_Material.IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
    else                         diffuse_coeff = u_Material.Diffuse;
    vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(u_CameraPosition - o_Pos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if (u_Material.IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
    else                          spec_coeff = u_Material.Specular;
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
    if (u_Material.IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
    else                         diffuse_coeff = u_Material.Diffuse;
    vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

    // specular
    vec3 view_dir = normalize(u_CameraPosition - o_Pos);
    vec3 reflect_dir = reflect(-light_dir, Normal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.Shininess);
    vec3 spec_coeff = vec3(0, 0, 0);
    if (u_Material.IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
    else                          spec_coeff = u_Material.Specular;
    vec3 specular = Light.Color * (spec_strength * spec_coeff);

    return ambient + diffuse + specular;
}

vec3 SpotLightShade(spot_light Light, vec3 Normal)
{
    vec3 light_dir = normalize(Light.Position - o_Pos);
    float theta = dot(light_dir, normalize(-Light.Direction));
    float intensity = clamp((theta - Light.OuterCutoff) / Light.Epsilon, 0.0, 1.0);

    if(intensity > 0) 
    {
        // ambient
        vec3 ambient = Light.Color * u_EnviromentAmbient;

        // diffuse
        vec3 light_dir = normalize(Light.Position - o_Pos);
        float diffuse_strength = max(dot(Normal, light_dir), 0.0);
        vec3 diffuse_coeff = vec3(0, 0, 0);
        if (u_Material.IsDiffuseMap) diffuse_coeff = texture(u_DiffuseMap, o_TexCoords).rgb;
        else                         diffuse_coeff = u_Material.Diffuse;
        vec3 diffuse = Light.Color * (diffuse_strength * diffuse_coeff);

        // specular
        vec3 view_dir = normalize(u_CameraPosition - o_Pos);
        vec3 reflect_dir = reflect(-light_dir, Normal);
        float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.Shininess);
        vec3 spec_coeff = vec3(0, 0, 0);
        if (u_Material.IsSpecularMap) spec_coeff = texture(u_SpecularMap, o_TexCoords).rgb;
        else                          spec_coeff = u_Material.Specular;
        vec3 specular = Light.Color * (spec_strength * spec_coeff);

        return ambient + (diffuse + specular) * intensity;
    }
    else
      if (u_Material.IsDiffuseMap) return u_EnviromentAmbient * texture(u_DiffuseMap, o_TexCoords).xyz;
      else                         return u_EnviromentAmbient * u_Material.Diffuse;
}

/**********************************************
 * Distance     Constant   Linear   Quadratic
 *    0007        1.0       0.7       1.8
 *    0013        1.0       0.35      0.44
 *    0020        1.0       0.22      0.20
 *    0032        1.0       0.14      0.07
 *    0050        1.0       0.09      0.032
 *    0065        1.0       0.07      0.017
 *    0100        1.0       0.045     0.0075
 *    0160        1.0       0.027     0.0028
 *    0200        1.0       0.022     0.0019
 *    0325        1.0       0.014     0.0007
 *    0600        1.0       0.007     0.0002
 *    3250        1.0       0.0014    0.000007
 */

void main()
{
    vec3 norm = normalize(o_Normal);

    point_light Point = point_light(vec3(2, 4, 2), vec3(1, 0, 0), 1.0, 0.045, 0.0075);
    directional_light Dir = directional_light(vec3(-0.2f, -1.0f, -0.3f), vec3(0.4, 0.4, 0.4));
    spot_light Spot = spot_light(vec3(-5, 2, -5), vec3(1, -1, 0.7), vec3(0, 1, 0), 0.91,0.82, 0.09);
    OutColor = vec4(PointLightShade(Point, norm) +
                    DirectionalLightShade(Dir, norm) + 
                    SpotLightShade(Spot, norm), 1);
}