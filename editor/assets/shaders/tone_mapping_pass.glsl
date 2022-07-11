#version 460

#shader-begin vert
    void main( void )
    {
      gl_Position = vec4(0, 0, 0, 1);
    }
#shader-end

#shader-begin geom
    layout (points) in;
    layout (triangle_strip, max_vertices = 4) out;

    out vec2 TexCoords;

    void main( void )
    {
        gl_Position = vec4(-1, 1, 0, 1);
        TexCoords = vec2(0, 1);
        EmitVertex();

        gl_Position = vec4(-1, -1, 0, 1);
        TexCoords = vec2(0, 0);
        EmitVertex();

        gl_Position = vec4(1, 1, 0, 1);
        TexCoords = vec2(1, 1);
        EmitVertex();

        gl_Position = vec4(1, -1, 0, 1);
        TexCoords = vec2(1, 0);
        EmitVertex();

        EndPrimitive();
    }
#shader-end

#shader-begin frag
    #include "binding_points.include.glsl"

    layout(binding  = TEXTURE_SLOT_HDR_BUFFER) uniform sampler2D u_HDRBuffer;
    layout(binding  = TEXTURE_SLOT_BLUR_BUFFER) uniform sampler2D u_BloomBuffer;
    in vec2 TexCoords;
    uniform float u_Exposure;

    out vec4 OutColor;

    void main()
    {
        vec4 hdr_color = texture(u_HDRBuffer, TexCoords) + texture(u_BloomBuffer, TexCoords);
        vec3 result = vec3(1.0) - exp(-hdr_color.rgb * u_Exposure);
        OutColor = vec4(result, hdr_color.w);
    }
#shader-end