/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#include "resources/textured.hpp"
#include "resources/faces.hpp"
#include "resources/text.hpp"

GLOBAL constexpr const char* SCREENSAVER_TITLE = "Feelies";

GLOBAL Vertex_Buffer faces_buffer;
GLOBAL Vertex_Buffer text_buffer;

GLOBAL Texture faces;
GLOBAL Texture text;

GLOBAL Shader textured;

GLOBAL mat4 projection;
GLOBAL mat4 modelview;

struct Vertex
{
    vec2 pos;
    vec2 tex;
};

GLOBAL Vertex faces_vert[4];
GLOBAL Vertex text_vert[4];

GLOBAL float sin_counter;

GLOBAL float tex_offset_x;
GLOBAL float tex_offset_y;

STDDEF bool screensaver_init ()
{
    if (!load_texture_from_mem(faces, FACES, STATIC_ARRAY_COUNT(FACES))) return false;
    if (!load_texture_from_mem(text, TEXT, STATIC_ARRAY_COUNT(TEXT))) return false;

    if (!create_vertex_buffer(faces_buffer)) return false;
    if (!create_vertex_buffer(text_buffer)) return false;

    if (!load_shader_from_mem(textured, TEXTURED, STATIC_ARRAY_COUNT(TEXTURED))) return false;

    faces_buffer.mode = DRAW_TRIANGLE_STRIP;
    text_buffer.mode = DRAW_TRIANGLE_STRIP;

    set_buffer_attrib(faces_buffer, 0, 2, false, 4, 0);
    set_buffer_attrib(faces_buffer, 1, 2, false, 4, 2);
    set_buffer_attrib(text_buffer , 0, 2, false, 4, 0);
    set_buffer_attrib(text_buffer , 1, 2, false, 4, 2);

    float tsx = get_window_w() / faces.w;
    float tsy = get_window_h() / faces.h;

    float tw = text.w;
    float th = text.h;

    faces_vert[0] = {{       -1.0f ,        1.0f  }, { 0.0f,  tsy }};
    faces_vert[1] = {{       -1.0f ,       -1.0f  }, { 0.0f, 0.0f }};
    faces_vert[2] = {{        1.0f ,        1.0f  }, {  tsx,  tsy }};
    faces_vert[3] = {{        1.0f ,       -1.0f  }, {  tsx, 0.0f }};
    text_vert [0] = {{ -(tw / 2.0f),  (th / 2.0f) }, { 0.0f, 1.0f }};
    text_vert [1] = {{ -(tw / 2.0f), -(th / 2.0f) }, { 0.0f, 0.0f }};
    text_vert [2] = {{  (tw / 2.0f),  (th / 2.0f) }, { 1.0f, 1.0f }};
    text_vert [3] = {{  (tw / 2.0f), -(th / 2.0f) }, { 1.0f, 0.0f }};

    set_buffer_data(faces_buffer, faces_vert, sizeof(faces_vert), BUFFER_STATIC);
    set_buffer_data(text_buffer,  text_vert,  sizeof(text_vert),  BUFFER_STATIC);

    return true;
}

STDDEF void screensaver_quit ()
{
    free_vertex_buffer(faces_buffer);
    free_vertex_buffer(text_buffer);

    free_texture(faces);
    free_texture(text);

    free_shader(textured);
}

STDDEF float sin_range (float min, float max, float t)
{
    float half_range = (max - min) / 2;
    return min + half_range + sinf(t) * half_range;
}

STDDEF void screensaver_update (float dt)
{
    constexpr float SPEED       = 128.0f;
    constexpr float FREQUENCY   =   2.2f;
    constexpr float MAGNITUDE   =   8.0f;
    constexpr float SCALE_SPEED =   2.0f;
    constexpr float SCALE_POWER =   4.0f;

    use_shader(textured);

    orthographic(projection, -1, 1, -1, 1);
    identity(modelview);

    set_uniform_mat4fv(textured, "projection", 1, false, &projection[0][0]);
    set_uniform_mat4fv(textured, "modelview", 1, false, &modelview[0][0]);

    tex_offset_x = -(sinf(sin_counter) * SPEED / 4.0f);
    tex_offset_y = tex_offset_y + SPEED * dt;

    if      (tex_offset_x <= -(faces.w)) tex_offset_x += faces.w;
    else if (tex_offset_y >          0 ) tex_offset_x -= faces.w;
    if      (tex_offset_y >=   faces.h ) tex_offset_y -= faces.h;

    float txoff = tex_offset_x / faces.w;
    float tyoff = tex_offset_y / faces.h;

    set_uniform_2f(textured, "tex_offset", txoff, tyoff);

    use_texture(faces);
    set_texture_wrap(TEXTURE_WRAP_REPEAT);
    draw_buffer(faces_buffer, 4);

    float ww = get_window_w();
    float wh = get_window_h();

    float l = -(ww / 2.0f);
    float r =  (ww / 2.0f);
    float b = -(wh / 2.0f);
    float t =  (wh / 2.0f);

    orthographic(projection, l, r, b, t);
    identity(modelview);

    float rad = toradf(((360.0f * (M_PIf / 180.0f)) / MAGNITUDE) + (sinf(sin_counter) * MAGNITUDE));
    float scl = sinf(sin_counter * SCALE_SPEED) * (MAGNITUDE * SCALE_POWER);

    sin_counter += FREQUENCY * dt;

    float sx = (text.w - scl) / text.w;
    float sy = (text.h - scl) / text.h;

    scale(modelview, sx, sy);
    rotate(modelview, 0, 0, 1, rad);

    set_uniform_mat4fv(textured, "projection", 1, false, &projection[0][0]);
    set_uniform_mat4fv(textured, "modelview", 1, false, &modelview[0][0]);

    set_uniform_2f(textured, "tex_offset", 0.0f, 0.0f);

    use_texture(text);
    set_texture_wrap(TEXTURE_WRAP_CLAMP_TO_EDGE);
    draw_buffer(text_buffer, 4);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/
