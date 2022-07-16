#version 460

#shader-begin vert
    /* Shader input data */
    #include "lib/default_vertex_layout.include.glsl"

    /* Shader output data */
    out vec2 TexCoords;

    void main()
    {
        TexCoords = v_TexCoords;
        gl_Position = vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    /* Shader input data. */
    uniform ivec2 u_MousePos;
    uniform ivec2 u_ScreenSize;
    uniform float u_Time;
    in vec2 TexCoords;

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;

    void main()
    {
        float point_dist = length(gl_FragCoord.xy - vec2(u_MousePos.x, u_ScreenSize.y - u_MousePos.y));
        float ball_dist = length(gl_FragCoord.xy - vec2(40 +abs(sin(u_Time)) * 500, 200 + cos(u_Time * 3 + 2) * 50));
        float floating_ball_dist = length(gl_FragCoord.xy - vec2(500 + sin(u_Time * 2) * 200, 400 + cos(u_Time) * 200));
        float line_dist = abs(gl_FragCoord.x) / sqrt(2);
        float color = pow(0.9, line_dist) * 50 + pow(0.92, point_dist) * 15 + 1 / ball_dist * 10 + 14 / floating_ball_dist;
        OutColor = vec4(vec3(color) * 5, 1);
    }
#shader-end