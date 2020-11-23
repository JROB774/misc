/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#include "resources/effect.hpp"
#include "resources/skull.hpp"

GLOBAL constexpr const char* SCREENSAVER_TITLE = "Skull";

GLOBAL Vertex_Buffer vertex_buffer;

GLOBAL Texture texture;
GLOBAL Shader shader;

GLOBAL mat4 proj;

GLOBAL float spaz_time_timer;
GLOBAL bool  spaz_time;

GLOBAL float random_counter;
GLOBAL float poll_random;

GLOBAL float random_x;
GLOBAL float random_y;

GLOBAL struct Vertex
{
    vec2 pos;
    vec2 tex;

} vert[4];

STDDEF bool screensaver_init ()
{
    if (!load_texture_from_mem(texture, SKULL, STATIC_ARRAY_COUNT(SKULL))) return false;
    if (!load_shader_from_mem(shader, EFFECT, STATIC_ARRAY_COUNT(EFFECT))) return false;

    if (!create_vertex_buffer(vertex_buffer)) return false;

    vertex_buffer.mode = DRAW_TRIANGLE_STRIP;

    set_buffer_attrib(vertex_buffer, 0, 2, false, 4, 0);
    set_buffer_attrib(vertex_buffer, 1, 2, false, 4, 2);

    vert[0] = { { -0.8f,  0.8f }, { 0.0f, 1.0f } };
    vert[1] = { { -0.8f, -0.8f }, { 0.0f, 0.0f } };
    vert[2] = { {  0.8f,  0.8f }, { 1.0f, 1.0f } };
    vert[3] = { {  0.8f, -0.8f }, { 1.0f, 0.0f } };

    set_buffer_data(vertex_buffer, vert, sizeof(vert), BUFFER_STATIC);

    float ww = get_window_w();
    float wh = get_window_h();

    float aspect = ww / wh;

    float l = -aspect;
    float r =  aspect;
    float b = -1.0f;
    float t =  1.0f;

    orthographic(proj, l, r, b, t);

    return true;
}

STDDEF void screensaver_quit ()
{
    free_vertex_buffer(vertex_buffer);

    free_texture(texture);
    free_shader(shader);
}

STDDEF float random_float (float a, float b)
{
    float rnd = ((float)random_int()) / (float)RAND_MAX;
    float diff = b - a;
    float r = rnd * diff;
    return a + r;
}

STDDEF void screensaver_update (float dt)
{
    clear_screen(1,1,1,1);

    use_texture(texture);
    use_shader(shader);

    set_uniform_mat4fv(shader, "projection", 1, false, &proj[0][0]);

    constexpr float POLL_RANDOM = 0.02f; // seconds
    constexpr float RANDOM_TIME = 0.03f; // seconds

    poll_random += dt;
    if (poll_random >= POLL_RANDOM)
    {
        random_counter += dt;
        if (random_counter >= RANDOM_TIME)
        {
            int random_val = random_int() % 10;
            if (spaz_time || random_val == 1)
            {
                random_x = random_float(-0.04f, 0.04f);
                random_y = random_float(-0.04f, 0.04f);

                int spaz_val = random_int() % 10;
                if (spaz_val == 1)
                {
                    spaz_time_timer += dt;
                    spaz_time = true;
                    if (spaz_time_timer > random_float(0.05f, 0.1f))
                    {
                        spaz_time_timer = 0.0f;
                        spaz_time = false;
                    }
                }
            }
            else
            {
                random_x = random_float(-0.015f, 0.015f);
                random_y = random_float(-0.015f, 0.015f);
            }

            random_counter -= RANDOM_TIME;
        }

        poll_random -= POLL_RANDOM;
    }

    set_uniform_2f(shader, "random", random_x, random_y);

    draw_buffer(vertex_buffer, 4);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/
