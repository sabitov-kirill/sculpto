#version 460

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
    #include "binding_points.include.glsl"

    layout(binding  = TEXTURE_SLOT_LIGHTING_PASS_OUT_COLOR) uniform sampler2D u_HDRBuffer;
    in vec2 TexCoords;
    uniform float u_Exposure;

    out vec4 OutColor;

    void main()
    {
        vec4 hdr_color = texture(u_HDRBuffer, TexCoords);
        vec3 result = vec3(1.0) - exp(-hdr_color.rgb * u_Exposure);
        OutColor = vec4(result, hdr_color.w);
    }
#shader-end