#version 330

uniform samplerCube skybox;

uniform mat4 projection;
uniform mat4 view;

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec3 in_pos;
    out vec3 texcoord;

    void vert(){
        gl_Position = vec4(projection * view * vec4(in_pos, 1.0f)).xyww; // Maximum depth so we can draw skybox last.
        texcoord = in_pos;
    }
#elif COMPILING_FRAGMENT_PROGRAM
    in vec3 texcoord;
    out vec4 frag_color;

    void frag(){
        frag_color = texture(skybox, texcoord);
    }
#endif
