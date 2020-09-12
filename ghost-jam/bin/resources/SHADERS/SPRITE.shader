#version 330

uniform sampler2D texture0;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// @Incomplete: Make this a uniform!
#if 0
const float dropoff = 20.0f;
#else
const float dropoff = 512.0f;
#endif

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec3 in_pos;
    layout (location = 1) in vec2 in_texcoord;
    layout (location = 2) in vec4 in_color;

    out vec2 texcoord;
    out vec4 vert_color;

    out float depth;

    void vert(){
        mat4 modelview = view * model;

        // For some reason it doesn't work when there's no block scope?!
        {
        modelview[0][0] = 1.0f;
        modelview[0][1] = 0.0f;
        modelview[0][2] = 0.0f;
        }

        gl_Position = projection * modelview * vec4(in_pos, 1.0f);
        texcoord = in_texcoord;
        vert_color = in_color;
        depth = (dropoff - gl_Position.z) / dropoff;
    }
#elif COMPILING_FRAGMENT_PROGRAM
    in vec2 texcoord;
    in vec4 vert_color;

    in float depth;

    out vec4 frag_color;

    void frag(){
        vec4 object_color = texture(texture0, texcoord) * vert_color;
        vec4 light_color = vec4(1.0f, 0.95f, 0.9f, 1.0f); // @Incomplete: Make this a uniform!

        object_color.rgb *= depth;

        frag_color = light_color * object_color;
    }
#endif
