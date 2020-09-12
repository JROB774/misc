#version 330

uniform sampler2D framebuffer;

const float resolution = 4.5; // the resolution of the selected colors (lower = more limited)
const float saturation = 1.5; // how saturated the resulting colors should be

// const vec2 mosaic_size = vec2(500.0, 500.0); // the smaller the number the larger the on-screen pixels

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec2 in_pos;
    layout (location = 1) in vec2 in_texcoord;

    out vec2 texcoord;

    void vert(){
        gl_Position = vec4(in_pos, 0.0f, 1.0f);
        texcoord = in_texcoord;
    }
#elif COMPILING_FRAGMENT_PROGRAM
    in vec2 texcoord;
    out vec4 frag_color;

    void frag(){
        //vec2 screen_fract_coords = fract(texcoord.xy * mosaic_size) / mosaic_size;
        frag_color = texture(framebuffer, texcoord.xy/* - screen_fract_coords*/);

        vec3 fract_pixel = frag_color.rgb - fract(frag_color.rgb * resolution) / resolution;

        float luma = dot(fract_pixel, vec3(0.3, 0.59, 0.11));
        vec3 chroma = (fract_pixel - luma) * saturation;

        frag_color.rgb = luma + chroma;
        frag_color.a = 1.0;
    }
#endif
