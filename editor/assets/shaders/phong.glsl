#version 460

#include "binding_points.glsl"
#include "lights_data.glsl"
#include "scene_data.glsl"

#shader-begin vert
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
#shader-end

#shader-begin frag
    #include "material_data.glsl"

    vec3 u_EnviromentAmbient = vec3(0.03, 0.02, 0.025);

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
        vec3 proj_coords = ShadowLightSpacePos.xyz / ShadowLightSpacePos.w  * 0.5 + 0.5;
        if(proj_coords.z > 1.0)
            return 0.0;

        float closest_depth = texture(u_ShadowMap, proj_coords.xy).r;
        float current_depth = proj_coords.z;

        float bias = 0.005;

        // PCF
        float shadow = 0.0;
        vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
        for(int x = -1; x <= 1; ++x)
            for(int y = -1; y <= 1; ++y)
            {
                float pcf_depth = texture(u_ShadowMap, proj_coords.xy + vec2(x, y) * texelSize).r; 
                shadow += current_depth - bias > pcf_depth ? 0.8 : 0.0;
            }
        shadow /= 9.0;
        return shadow;

        // return current_depth - bias > closest_depth ? 0.8 : 0.0;
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
#shader-end
