#version 460

#include "binding_points.include.glsl"

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
    layout(binding = TEXTURE_SLOT_APPLY_SOURCE) uniform sampler2D u_TextureSource;
    layout(binding = TEXTURE_SLOT_APPLY_TEXTURE_ADD) uniform sampler2D u_TextureAdd;
    in vec2 TexCoords;

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;

    void main()
    {
        OutColor = texture(u_TextureSource, TexCoords) + texture(u_TextureAdd, TexCoords);
    }
#shader-end