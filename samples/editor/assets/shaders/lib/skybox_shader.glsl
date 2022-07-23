#version 460

#include "binding_points.include.glsl"

#shader-begin vert
    #include "default_vertex_layout.include.glsl"

    uniform mat4 u_MatrWVP;
    out vec2 vert_out_TexCoords;

    void main()
    {
        vert_out_TexCoords = v_TexCoords;
        gl_Position  = vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    #include "pipeline_data.include.glsl"
    #include "bloom.include.glsl"

    #define PI 3.14159265359

    /* Currently rendering mesh material data. */
    layout(binding = TEXTURE_SLOT_MATERIAL_DIFFUSE) uniform sampler2D u_Texture;
    in vec2 vert_out_TexCoords;

    /* Shader output data. */
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_POSITION       ) out vec4 OutPosition;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_NORMAL         ) out vec4 OutNormal;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_COLOR          ) out vec4 OutColor;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_DIFFUSE  ) out vec4 OutDiffuse;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SPECULAR ) out vec4 OutSpecular;
    layout(location = COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SHININESS) out vec4 OutShininessIsShadeIsBloomed;

    void main()
    {
        float w = u_ViewportWidth, h = u_ViewportHeight;
        vec2 s = (vert_out_TexCoords - 0.5) * 1;

        if (w > h) s.x *= w / h;
        else s.y *= h / w;

        vec3 dir = normalize(u_CameraDirection + u_CameraRightDirection * s.x + u_CameraUpDirection * s.y);
        float theta = -acos(dir.y);
        float phi = atan(dir.x, -dir.z);
        vec2 c = vec2(phi / 2 / PI, theta / PI);

        vec4 tc = texture(u_Texture, c);
        OutColor = vec4(tc.rgb, 1);
        OutDiffuse = vec4(0, 0, 0, 1);
        OutShininessIsShadeIsBloomed = vec4(0, 0, 0, 1);
    }
#shader-end