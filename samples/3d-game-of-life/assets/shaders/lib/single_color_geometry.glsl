#version 460

#include "binding_points.include.glsl"

#shader-begin vert
    #include "default_vertex_layout.include.glsl"

    uniform mat4 u_MatrWVP;
    out vec2 vert_out_TexCoords;

    void main()
    {
        vert_out_TexCoords = v_TexCoords;
        gl_Position  = u_MatrWVP * vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    #include "pipeline_data.include.glsl"
    #include "bloom.include.glsl"

    /* Currently rendering mesh material data. */
    layout(std140, binding = BINDING_POINT_MATERIAL_DATA) uniform ubo_Material {
        vec3 Color;
        bool IsTexture;
    };
    layout(binding = TEXTURE_SLOT_MATERIAL_DIFFUSE) uniform sampler2D u_Texture;
    in vec2 vert_out_TexCoords;

    /* Shader output data. */
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_POSITION       ) out vec4 OutPosition;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_NORMAL         ) out vec4 OutNormal;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_COLOR          ) out vec4 OutColor;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_DIFFUSE  ) out vec4 OutDiffuse;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SPECULAR ) out vec4 OutSpecular;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SHININESS) out vec4 OutShininess;

    void main()
    {
        if (IsTexture) OutColor = texture(u_Texture, vert_out_TexCoords);
        else           OutColor = vec4(Color, 1);
        OutDiffuse = vec4(0, 0, 0, 1);
    }
#shader-end