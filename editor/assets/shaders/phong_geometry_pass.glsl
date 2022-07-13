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

    /* Shader output data. */
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_POSITION       ) out vec4 OutPosition;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_NORMAL         ) out vec4 OutNormal;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_COLOR          ) out vec4 OutColor;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_DIFFUSE  ) out vec4 OutDiffuse;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SPECULAR ) out vec4 OutSpecular;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SHININESS) out vec4 OutShininess;

    void main()
    {
        OutPosition = vec4(fs_in.Pos, 1);

        vec3 norm = vec3(0);
        if (IsNormalMap) norm = fs_in.TBN * (texture(u_NormalMap, fs_in.TexCoords).rgb * 2 - 1);
        else             norm = fs_in.Normal;
        OutNormal = vec4(normalize(norm), 1);

        if (IsEmissionMap) OutColor = vec4(texture(u_EmissionMap, fs_in.TexCoords).rgb * 5, 1);

        if (IsDiffuseMap) OutDiffuse = texture(u_DiffuseMap, fs_in.TexCoords);
        else              OutDiffuse = vec4(Diffuse, 1);
        if (OutDiffuse.w < 0.1) discard;

        if (IsSpecularMap) OutSpecular = vec4(texture(u_SpecularMap, fs_in.TexCoords).rgb, 1);
        else               OutSpecular = vec4(Specular, 1);

        OutShininess = vec4(Shininess, 0, 0, 1);
    }
#shader-end
