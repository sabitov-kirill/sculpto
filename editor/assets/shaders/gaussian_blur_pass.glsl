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

    layout(binding  = TEXTURE_SLOT_BLUR_BUFFER) uniform sampler2D u_BlurBuffer;
    uniform bool u_IsHorizontal;
    uniform float u_Weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    in vec2 TexCoords;

    out vec4 OutColor;

    void main()
    {
        vec2 tex_offset = 1.0 / textureSize(u_BlurBuffer, 0);
        vec3 result = texture(u_BlurBuffer, TexCoords).rgb * u_Weight[0];
        if (u_IsHorizontal)
        {
            for(int i = 1; i < 5; ++i)
            {
                result += texture(u_BlurBuffer, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * u_Weight[i];
                result += texture(u_BlurBuffer, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * u_Weight[i];
            }
        }
        else
        {
            for(int i = 1; i < 5; ++i)
            {
                result += texture(u_BlurBuffer, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * u_Weight[i];
                result += texture(u_BlurBuffer, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * u_Weight[i];
            }
        }
        OutColor = vec4(result, 1.0);
    }
#shader-end