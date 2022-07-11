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
        float IsTexture;
    };
    layout(binding = TEXTURE_SLOT_MATERIAL_DIFFUSE) uniform sampler2D u_Texture;
    in vec2 vert_out_TexCoords;

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;
    layout(location = 1) out vec4 BrightColor;

    void main()
    {
        if (IsTexture) OutColor = vec4(texture(u_Texture, vert_out_TexCoords).rgb, 1);
        else           OutColor = vec4(Color, 1);
        if (IsBloomActive && IsColorBright(OutColor.rgb))
            BrightColor = vec4(OutColor.rgb, 1.0);
    }
#shader-end