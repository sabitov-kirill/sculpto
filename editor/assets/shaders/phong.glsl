#version 460

#include "binding_points.include.glsl"
#include "pipeline_data.include.glsl"
#include "phong_lights_data.include.glsl"
#include "phong_material_data.include.glsl"

#shader-begin vert
    #include "default_vertex_layout.include.glsl"

    uniform mat3 u_MatrN;
    uniform mat4 u_MatrW;
    uniform mat4 u_MatrWVP;

    /* Shader output data. */
    out VS_OUT
    {
        vec3 Pos;
        vec4 LightSpacePos;
        vec3 Normal;
        mat3 TBN;
        vec2 TexCoords;
    } vs_out;

    void main()
    {
        vs_out.TexCoords = v_TexCoords;
        vs_out.Pos = vec3(u_MatrW * vec4(v_Pos, 1.0));
        if (DirectionalLight.IsShadows)
            vs_out.LightSpacePos = DirectionalLight.ViewProjection * vec4(vs_out.Pos, 1.0);
        vs_out.Normal = normalize(u_MatrN * v_Normal);
        if (IsNormalMap)
            vs_out.TBN = mat3(normalize(u_MatrN * v_Tangent), normalize(u_MatrN * v_Bitangent), vs_out.Normal);

        gl_Position = u_MatrWVP * vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    #include "phong_color_calculation.include.glsl"
    #include "bloom.include.glsl"

    /* Shader input data. */
    in VS_OUT
    {
        vec3 Pos;
        vec4 LightSpacePos;
        vec3 Normal;
        mat3 TBN;
        vec2 TexCoords;
    } fs_in;

    vec3 PointLightShade(point_light _Light, vec3 _Normal, vec3 _Diffuse, vec3 _Specular, float _Shininess)
    {
        float dist = length(_Light.Position - fs_in.Pos);
        float attenuation = 1.0 / (_Light.Constant + _Light.Linear * dist + _Light.Quadratic * (dist * dist));
        vec3 light_dir = normalize(_Light.Position - fs_in.Pos);

        vec3 diffuse = DiffuseColorCalculate(light_dir, _Light.Color, _Normal, _Diffuse);
        vec3 specular = SpecularColorCalculate(light_dir, _Light.Color, CameraPosition, fs_in.Pos, _Normal, _Specular, _Shininess);

        return _Light.Color * (diffuse + specular) * attenuation;
    }

    vec3 DirectionalLightShade(directional_light _Light, vec3 _Normal, vec3 _Diffuse, vec3 _Specular, float _Shininess)
    {
        vec3 light_dir = normalize(-_Light.Direction);

        vec3 diffuse = DiffuseColorCalculate(light_dir, _Light.Color, _Normal, _Diffuse);
        vec3 specular = SpecularColorCalculate(light_dir, _Light.Color, CameraPosition, fs_in.Pos, _Normal, _Specular, _Shininess);

        if (_Light.IsShadows)
        {
            float shadow = ShadowFactorCalculate(fs_in.LightSpacePos, u_ShadowMap);
            return _Light.Color * (1 - shadow) * (diffuse + specular);
        }
        return vec3(0);
    }

    vec3 SpotLightShade(spot_light _Light, vec3 _Normal, vec3 _Diffuse, vec3 _Specular, float _Shininess)
    {
        vec3 light_dir = normalize(_Light.Position - fs_in.Pos);
        float theta = dot(light_dir, normalize(-_Light.Direction));
        float intensity = clamp((theta - _Light.OuterCutoffCos) / _Light.Epsilon, 0.0, 1.0);

        vec3 diffuse = DiffuseColorCalculate(light_dir, _Light.Color, _Normal, _Diffuse);
        if(intensity >= 0) 
        {
            vec3 specular = SpecularColorCalculate(light_dir, _Light.Color, CameraPosition, fs_in.Pos, _Normal, _Specular, _Shininess);
            return _Light.Color * (diffuse + specular) * intensity;
        }
        return vec3(0);
    }

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;
    layout(location = 1) out vec4 BrightColor;

    void main()
    {
        vec3 norm = vec3(0);
        if (IsNormalMap) norm = fs_in.TBN * (texture(u_NormalMap, fs_in.TexCoords).rgb * 2 - 1);
        else             norm = fs_in.Normal;
        norm = normalize(norm);
        vec4 diffuse  = vec4(0);
        if (IsDiffuseMap) diffuse = texture(u_DiffuseMap, fs_in.TexCoords);
        else              diffuse = vec4(Diffuse, 1);
        vec3 specular = vec3(0);
        if (IsSpecularMap) specular = texture(u_SpecularMap, fs_in.TexCoords).rgb;
        else               specular = Specular;

        if (diffuse.w < 0.01) discard;

        vec3 color = vec3(0, 0, 0);
        for (uint i = 0; i < PointLightsCount; ++i) color += PointLightShade(PointLights[i],         norm, diffuse.rgb, specular, Shininess);
        for (uint i = 0; i < SpotLightsCount; ++i)  color += SpotLightShade(SpotLights[i],           norm, diffuse.rgb, specular, Shininess);
        if (IsDirectionalLight)                     color += DirectionalLightShade(DirectionalLight, norm, diffuse.rgb, specular, Shininess);
        color += diffuse.rgb * EnviromentAmbient;
        if (IsEmissionMap) color += texture(u_EmissionMap, fs_in.TexCoords).rgb * 10;

        OutColor = vec4(color, diffuse.w);
        if (IsBloomActive && IsColorBright(color))
            BrightColor = vec4(color, 1.0);
    }
#shader-end
