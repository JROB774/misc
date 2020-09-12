#version 330

uniform sampler2D texture0;

uniform mat4 projection;
uniform mat4 view;
uniform vec4 color;

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec2 in_pos;
    layout (location = 1) in vec2 in_texcoord;

    out vec2 texcoord;

    void vert(){
        gl_Position = projection * view * vec4(in_pos, 0.0f, 1.0f);
        texcoord = in_texcoord;
    }
#elif COMPILING_FRAGMENT_PROGRAM
    in vec2 texcoord;
    out vec4 frag_color;

    void frag(){
        frag_color = texture(texture0, texcoord) * color;
    }
#endif
