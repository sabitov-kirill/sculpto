#version 460

#include "binding_points.include.glsl"
#include "pipeline_data.include.glsl"
#include "phong_lights_data.include.glsl"
#include "phong_material_data.include.glsl"

#shader-begin vert
    #include "default_vertex_layout.include.glsl"

    out vec2 TexCoords;

    void main()
    {
        TexCoords = v_TexCoords;
        gl_Position = vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    /* Shader input data. */
    layout(binding = TEXTURE_SLOT_GEOM_PASS_OUT_POSITION       ) uniform sampler2D InPosition;
    layout(binding = TEXTURE_SLOT_GEOM_PASS_OUT_NORMAL         ) uniform sampler2D InNormal;
    layout(binding = TEXTURE_SLOT_GEOM_PASS_OUT_COLOR          ) uniform sampler2D InColor;
    layout(binding = TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_DIFFUSE  ) uniform sampler2D InDiffuse;
    layout(binding = TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_SPECULAR ) uniform sampler2D InSpecular;
    layout(binding = TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_SHININESS) uniform sampler2D InShininessIsShadeIsBloomed;

    in vec2 TexCoords;

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;
    layout(location = 1) out vec4 BrightColor;

    #include "phong_color_calculation.include.glsl"
    #include "bloom.include.glsl"

    void main()
    {
        vec4 position   = texture(InPosition,  TexCoords);
        vec4 normal     = texture(InNormal,    TexCoords);
        vec4 color      = texture(InColor,     TexCoords);
        vec4 diffuse    = texture(InDiffuse,   TexCoords);
        vec4 specular   = texture(InSpecular,  TexCoords);
        vec3 shininess_is_shade_is_bloomed = texture(InShininessIsShadeIsBloomed, TexCoords).rgb;
        float shininess = shininess_is_shade_is_bloomed.r;
        bool is_shade = bool(shininess_is_shade_is_bloomed.g);
        bool is_bloomed = bool(shininess_is_shade_is_bloomed.b);

        if (diffuse.w < 0.01) discard;

        vec3 result = vec3(0, 0, 0);
        if (is_shade)
        {
            for (uint i = 0; i < u_PointLightsCount; ++i) result += PointLightShade(u_PointLights[i], position.rgb, normal.rgb, diffuse.rgb, specular.rgb, shininess);
            for (uint i = 0; i < u_SpotLightsCount; ++i)  result += SpotLightShade(u_SpotLights[i], position.rgb, normal.rgb, diffuse.rgb, specular.rgb, shininess);
            if (u_IsDirectionalLight) {
                vec4 light_space_pos = vec4(0);
                if (u_DirectionalLight.IsShadows) light_space_pos = u_DirectionalLight.ViewProjection * vec4(position.rgb, 1.0);
                result += DirectionalLightShade(u_DirectionalLight, light_space_pos, position.rgb, normal.rgb, diffuse.rgb, specular.rgb, shininess);
            }
            result += diffuse.rgb * u_EnviromentAmbient;
        }
        result += color.rgb * 5;

        OutColor = vec4(result, 1);
        if (u_IsBloom && is_bloomed && IsColorBright(result))
            BrightColor = vec4(result, 1.0);
    }
#shader-end