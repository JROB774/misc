#version 330

uniform mat4 projection;
uniform mat4 view;

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec2 in_pos;
    layout (location = 1) in vec4 in_color;

    out vec4 vert_color;

    void vert(){
        gl_Position = projection * view * vec4(in_pos, 0.0f, 1.0f);
        vert_cololr = in_color;
    }
#elif COMPILING_FRAGMENT_PROGRAM
    in vec4 vert_color;
    out vec4 frag_color;
    void frag(){
        frag_color = vert_color;
    }
#endif
