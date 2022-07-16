#version 460

#include "lib/binding_points.include.glsl"

#shader-begin vert
    #include "lib/default_vertex_layout.include.glsl"

    out vec2 TexCoords;

    void main()
    {
        TexCoords = v_TexCoords;
        gl_Position = vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    /* Shader input data. */
    layout(binding = 0) uniform sampler2D u_AccumulatedFrames;
    uniform uint u_AccumulatedFramesCount;
    in vec2 TexCoords;

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;

    void main()
    {
        vec3 accumulated_color = texture(u_AccumulatedFrames, TexCoords).rgb;
        accumulated_color /= float(u_AccumulatedFramesCount);
        accumulated_color = accumulated_color / (accumulated_color + vec3(1.0));
        accumulated_color = pow(accumulated_color, vec3(1.0 / 2.2));
        OutColor = vec4(accumulated_color, 1);
    }
#shader-end