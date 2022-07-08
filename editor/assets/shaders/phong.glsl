#version 460

#include "binding_points.glsl"
#include "scene_data.glsl"
#include "phong_lights_data.glsl"
#include "phong_material_data.glsl"

#shader-begin vert
#include "default_vertex_layout.glsl"

uniform mat3 u_MatrN;
uniform mat4 u_MatrW;
uniform mat4 u_MatrWVP;

/* Shader output data. */
out VS_OUT
{
    vec3 Pos;
    vec4 LightSpacePos;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec2 TexCoords;
} vs_out;

void main()
{
    vs_out.TexCoords = v_TexCoords;
    vs_out.Pos = vec3(u_MatrW * vec4(v_Pos, 1.0));
    if (DirectionalLight.IsShadows) {
        vs_out.LightSpacePos = DirectionalLight.ViewProjection * vec4(vs_out.Pos, 1.0);
    }
    vs_out.Normal = u_MatrN * v_Normal;
    if (IsNormalMap) {
        vs_out.Tangent   = u_MatrN * v_Tangent;
        vs_out.Bitangent = u_MatrN * v_Bitangent;
    }

    gl_Position = u_MatrWVP * vec4(v_Pos, 1.0);
}
#shader-end

#shader-begin frag
/* Shader input data. */
in VS_OUT
{
    vec3 Pos;
    vec4 LightSpacePos;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec2 TexCoords;
} fs_in;
vec3 u_EnviromentAmbient = vec3(0.03, 0.02, 0.025);

/* Shader output data. */
layout(location = 0) out vec4 OutColor;

/*****************************************
  * Phong lighting model colors calculation
  *****************************************/

vec3 AmbientColorCalculate(vec3 _LightColor, vec3 _EnviromentAmbient)
{
    return _LightColor * _EnviromentAmbient;
}

vec3 DiffuseColorCalculate(vec3 _LightDir, vec3 _LightColor,
                            vec3 _SurfaceNormal, vec2 _SurfaceTexCoords,
                            uint _ObjectIsDiffuseMap, sampler2D _ObjectDiffuseMap, vec3 _ObjectDiffuse)
{
    float diffuse_strength = max(dot(_SurfaceNormal, _LightDir), 0.0);

    vec3 diffuse_coeff = vec3(0, 0, 0);
    if (_ObjectIsDiffuseMap)
        diffuse_coeff = texture(_ObjectDiffuseMap, _SurfaceTexCoords).rgb;
    else
        diffuse_coeff =  Diffuse;

    return _LightColor * (diffuse_strength * diffuse_coeff);
}

vec3 SpecularColorCalculate(vec3 _LightDir, vec3 _LightColor, vec3 _CameraPosition,
                            vec3 _FragmentPosition, vec3 _SurfaceNormal, vec2 _SurfaceTexCoords,
                            uint _ObjectIsSpecularMap, sampler2D _ObjectSpecularMap, vec3 _ObjectSpecular, float _ObjectShininess)
{
    vec3 view_dir = normalize(_CameraPosition - _FragmentPosition);
    vec3 reflect_dir = reflect(-_LightDir, _SurfaceNormal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0), _ObjectShininess);

    vec3 spec_coeff = vec3(0, 0, 0);
    if (_ObjectIsSpecularMap)
        spec_coeff = texture(_ObjectSpecularMap, _SurfaceTexCoords).rgb;
    else
        spec_coeff = _ObjectSpecular;

    return _LightColor * (spec_strength * spec_coeff);
}

float ShadowFactorCalculate(vec4 _ShadowLightSpacePos, sampler2D _ShadowMap)
{
    vec3 proj_coords = _ShadowLightSpacePos.xyz / _ShadowLightSpacePos.w  * 0.5 + 0.5;
    if(proj_coords.z > 1.0)
        return 0.0;

    float closest_depth = texture(_ShadowMap, proj_coords.xy).r;
    float current_depth = proj_coords.z;

    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(_ShadowMap, proj_coords.xy + vec2(x, y) * texelSize).r; 
            shadow += current_depth - bias > pcf_depth ? 0.8 : 0.0;
        }
    shadow /= 9.0;
    return shadow;
}

/*********************************
  * Light casters color calculation
  *********************************/

vec3 PointLightShade(point_light Light, vec3 Normal)
{
    float dist = length(Light.Position - fs_in.Pos);
    float attenuation = 1.0 / (Light.Constant + Light.Linear * dist + Light.Quadratic * (dist * dist));
    vec3 light_dir = normalize(Light.Position - fs_in.Pos);

    vec3 ambient =  AmbientColorCalculate(Light.Color, u_EnviromentAmbient);
    vec3 diffuse = DiffuseColorCalculate(light_dir, Light.Color, Normal, fs_in.TexCoords, IsDiffuseMap, u_DiffuseMap, Diffuse);
    vec3 specular = SpecularColorCalculate(light_dir, Light.Color, CameraPosition, fs_in.Pos, Normal, fs_in.TexCoords, IsSpecularMap, u_SpecularMap, Specular, Shininess);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 DirectionalLightShade(directional_light Light, vec3 Normal)
{
    vec3 light_dir = normalize(-Light.Direction);

    vec3 ambient = AmbientColorCalculate(Light.Color, u_EnviromentAmbient);
    vec3 diffuse = DiffuseColorCalculate(light_dir, Light.Color, Normal, fs_in.TexCoords, IsDiffuseMap, u_DiffuseMap, Diffuse);
    vec3 specular = SpecularColorCalculate(light_dir, Light.Color, CameraPosition, fs_in.Pos, Normal, fs_in.TexCoords, IsSpecularMap, u_SpecularMap, Specular, Shininess);

    // shadows
    if (Light.IsShadows)
    {
        float shadow = ShadowFactorCalculate(fs_in.LightSpacePos, u_ShadowMap);
        return ambient + (1 - shadow) * (diffuse + specular);
    }
    return ambient + diffuse + specular;
}

vec3 SpotLightShade(spot_light Light, vec3 Normal)
{
    vec3 light_dir = normalize(Light.Position - fs_in.Pos);
    float theta = dot(light_dir, normalize(-Light.Direction));
    float intensity = clamp((theta - Light.OuterCutoffCos) / Light.Epsilon, 0.0, 1.0);

    if(intensity >= 0) 
    {
        vec3 ambient =  AmbientColorCalculate(Light.Color, u_EnviromentAmbient);
        vec3 diffuse = DiffuseColorCalculate(light_dir, Light.Color, Normal, fs_in.TexCoords, IsDiffuseMap, u_DiffuseMap, Diffuse);
        vec3 specular = SpecularColorCalculate(light_dir, Light.Color, CameraPosition, fs_in.Pos, Normal, fs_in.TexCoords, IsSpecularMap, u_SpecularMap, Specular, Shininess);

        return ambient + (diffuse + specular) * intensity;
    }
    return AmbientColorCalculate(Light.Color, u_EnviromentAmbient);
}



void main()
{
    vec3 norm = vec3(0, 0, 0);
    if (IsNormalMap) {
        vec3 norm_bump = texture(u_NormalMap, fs_in.TexCoords).rgb;
        mat3 tbn = mat3(fs_in.Tangent, fs_in.Bitangent, fs_in.Normal);
        norm = tbn * norm_bump;
    }
    else {
        norm = normalize(fs_in.Normal);
    }
    //OutColor = vec4(norm, 1);
    //return;

    vec3 color = vec3(0, 0, 0);
    for (uint i = 0; i < PointLightsCount; ++i) color += PointLightShade(PointLights[i], norm);
    for (uint i = 0; i < SpotLightsCount; ++i)  color += SpotLightShade(SpotLights[i], norm);
    if (IsDirectionalLight)                     color += DirectionalLightShade(DirectionalLight, norm);

    OutColor = vec4(color, 1);
}
#shader-end
