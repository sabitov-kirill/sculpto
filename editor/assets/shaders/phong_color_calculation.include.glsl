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
    vec3 reflect_dir = reflect(-_LightDir, _SurfaceNormal);
    float spec_strength = pow(max(dot(view_dir, reflect_dir), 0.0), _ObjectShininess);
    return (spec_strength * _ObjectSpecular);
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
            shadow += current_depth - bias > pcf_depth ? 1 : 0.0;
        }
    shadow /= 9.0;
    return shadow;
}