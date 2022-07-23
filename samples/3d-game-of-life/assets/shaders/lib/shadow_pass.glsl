#version 460

#shader-begin vert
    layout (location = 0) in vec3 v_Pos;
    uniform mat4 u_MatrWVP;

    void main()
    {
        gl_Position = u_MatrWVP * vec4(v_Pos, 1.0);
    }
#shader-end

#shader-begin frag
    void main() {}
#shader-end