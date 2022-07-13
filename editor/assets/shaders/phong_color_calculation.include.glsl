/*****************************************
  * Phong lighting model color components calculation
  *****************************************/

vec3 AmbientColorCalculate(vec3 _LightColor, vec3 _EnviromentAmbient)
{
    return _EnviromentAmbient;
}

vec3 DiffuseColorCalculate(vec3 _LightDir, vec3 _LightColor, vec3 _SurfaceNormal, vec3 _ObjectDiffuse)
{
    float diffuse_strength = max(dot(_SurfaceNormal, _LightDir), 0.0);
    return (diffuse_strength * _ObjectDiffuse);
}

vec3 SpecularColorCalculate(vec3 _LightDir, vec3 _LightColor, vec3 _CameraPosition,
                            vec3 _FragmentPosition, vec3 _SurfaceNormal,
                            vec3 _ObjectSpecular, float _ObjectShininess)
{
    vec3 view_dir = normalize(_CameraPosition - _FragmentPosition);
    // vec3 reflect_dir = reflect(-_LightDir, _SurfaceNormal);
    vec3 halfway_dir = normalize(-_LightDir + view_dir);
    float spec_strength = pow(max(dot(_SurfaceNormal, halfway_dir), 0.0), _ObjectShininess);
    return (spec_strength * _ObjectSpecular);
}

float ShadowFactorCalculate(vec4 _ShadowLightSpacePos, sampler2D _ShadowMap, vec3 _Normal, vec3 _LightDir)
{
    vec3 proj_coords = _ShadowLightSpacePos.xyz / _ShadowLightSpacePos.w  * 0.5 + 0.5;
    if(proj_coords.z > 1.0)
        return 0.0;

    float closest_depth = texture(_ShadowMap, proj_coords.xy).r;
    float current_depth = proj_coords.z;

    float bias = max(0.05 * (1.0 - dot(_Normal, _LightDir)), 0.005);  
    float shadow = 0.0;
    // shadow = current_depth - bias > closest_depth ? 1 : 0.0; return shadow;
    
    vec2 texel_size = 1.0 / textureSize(_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(_ShadowMap, proj_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf_depth ? 1 : 0.0;
        }
    shadow /= 9.0;
    return shadow;
}

/***************************************************
  * Light sources color impact calculation functions.
  ***************************************************/

vec3 PointLightShade(point_light _Light, vec3 _Position, vec3 _Normal, vec3 _Diffuse, vec3 _Specular, float _Shininess)
{
    float dist = length(_Light.Position - _Position);
    float attenuation = 1.0 / (_Light.Constant + _Light.Linear * dist + _Light.Quadratic * (dist * dist));
    vec3 light_dir = normalize(_Light.Position - _Position);

    vec3 diffuse = DiffuseColorCalculate(light_dir, _Light.Color, _Normal, _Diffuse);
    vec3 specular = SpecularColorCalculate(light_dir, _Light.Color, CameraPosition, _Position, _Normal, _Specular, _Shininess);

    return _Light.Color * (diffuse + specular) * attenuation;
}

vec3 DirectionalLightShade(directional_light _Light, vec4 _LightSpacePos, vec3 _Position, vec3 _Normal, vec3 _Diffuse, vec3 _Specular, float _Shininess)
{
    vec3 light_dir = normalize(-_Light.Direction);

    vec3 diffuse = DiffuseColorCalculate(light_dir, _Light.Color, _Normal, _Diffuse);
    vec3 specular = SpecularColorCalculate(light_dir, _Light.Color, CameraPosition, _Position, _Normal, _Specular, _Shininess);

    if (_Light.IsShadows)
    {
        float shadow = ShadowFactorCalculate(_LightSpacePos, u_ShadowMap, _Normal, light_dir);
        return _Light.Color * (1 - shadow) * (diffuse + specular);
    }
    return _Light.Color * (diffuse + specular);
}

vec3 SpotLightShade(spot_light _Light, vec3 _Position, vec3 _Normal, vec3 _Diffuse, vec3 _Specular, float _Shininess)
{
    vec3 light_dir = normalize(_Light.Position - _Position);
    float theta = dot(light_dir, normalize(-_Light.Direction));
    float intensity = clamp((theta - _Light.OuterCutoffCos) / _Light.Epsilon, 0.0, 1.0);

    vec3 diffuse = DiffuseColorCalculate(light_dir, _Light.Color, _Normal, _Diffuse);
    if(intensity >= 0)
    {
        vec3 specular = SpecularColorCalculate(light_dir, _Light.Color, CameraPosition, _Position, _Normal, _Specular, _Shininess);
        return _Light.Color * (diffuse + specular) * intensity;
    }
    return vec3(0);
}