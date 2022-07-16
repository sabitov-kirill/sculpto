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
    #define PI 3.1415926535

    /* Shader input data. */
    layout(binding = 0) uniform sampler2D u_AccumulatedFrames;
    layout(std140, binding = 0) uniform ubo_SceneData
    {
        vec3 u_CameraPosition;
        float u_CameraProjectionDistance;
        vec3 u_CameraDirection;
        float u_ViewportWidth;
        vec3 u_CameraRightDirection;
        float u_ViewportHeight;
        vec3 u_CameraUpDirection;
        float u_ViewportProjectionWidth;
        float u_ViewportProjectionHeight;
        float u_Time;
        uint u_Samples;
    };
    in vec2 TexCoords;

    /* Shader output data. */
    layout(location = 0) out vec4 OutColor;

    #include "raytracing_random.glsl"
    #include "raytracing_ray.glsl"
    #include "raytracing_objects.glsl"
    #include "raytracing_render.glsl"

    void main()
    {
        ray current_ray = RayFromCamera(gl_FragCoord.x, u_ViewportHeight - gl_FragCoord.y,
                                        u_CameraPosition, u_CameraDirection, u_CameraRightDirection, u_CameraUpDirection, u_CameraProjectionDistance,
                                        u_ViewportWidth, u_ViewportHeight, u_ViewportProjectionWidth, u_ViewportProjectionHeight);

        InitializeScene();

        vec3 color = vec3(0);
        for (uint i = 0; i < u_Samples; ++i)
        {
            float seed = sin(float(i) * u_Time);
            color += TracePath(current_ray, seed);
        }
        color /= float(u_Samples);
        color += texture(u_AccumulatedFrames, TexCoords).rgb;
        OutColor = vec4(color, 1);
    }
#shader-end