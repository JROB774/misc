#version 330

uniform mat4 projection;
uniform mat4 view;
uniform vec4 color;

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec2 in_pos;
    void vert(){
        gl_Position = projection * view * vec4(in_pos, 0.0f, 1.0f);
    }
#elif COMPILING_FRAGMENT_PROGRAM
    out vec4 frag_color;
    void frag(){
        frag_color = color;
    }
#endif
