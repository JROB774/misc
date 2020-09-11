#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "external/defer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "external/stb_image.h"

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef   int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;

static constexpr float FOV = ((float)(M_PI)) / 3.0f;

static constexpr int WIN_W = 1024;
static constexpr int WIN_H =  512;
static constexpr int MAP_W =   16;
static constexpr int MAP_H =   16;

static constexpr const char MAP[] =
"0000222222220000"
"1              0"
"1      11111   0"
"1     0        0"
"0     0  1110000"
"0     3        0"
"0   10000      0"
"0   0   11100  0"
"0   0   0      0"
"0   0   1  00000"
"0       1      0"
"2       1      0"
"0       0      0"
"0 0000000      0"
"0              0"
"0002222222200000";

struct Texture
{
    std::vector<u32> pixels;
    int w,h;
    int count; // Number of individual textures.
    int size;  // Size of the textures.
};

struct Image
{
    std::vector<u32> pixels;
    int w,h;
};

static void unpack_color (u32 color, u8& r, u8& g, u8& b, u8& a)
{
    r = (color >>  0) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = (color >> 16) & 0xFF;
    a = (color >> 24) & 0xFF;
}

static u32 pack_color (u8 r, u8 g, u8 b, u8 a = 0xFF)
{
    return ((a << 24) + (b << 16) + (g << 8) + (r));
}

static void draw_rect (Image& image, int x, int y, int w, int h, u32 color)
{
    assert((image.w*image.h) == image.pixels.size());

    for (int ix=0; ix<w; ++ix)
    {
        for (int iy=0; iy<h; ++iy)
        {
            int cx = x+ix, cy = y+iy;
            if ((cx >= image.w) || (cy >= image.h)) continue; // No need to draw negative values.
            image.pixels[cy * image.w + cx] = color;
        }
    }
}

static bool load_texture (const std::string file_name, Texture& texture)
{
    constexpr int BPP = 4;
    int w,h,bpp;
    u8* data = stbi_load(file_name.c_str(), &w,&h,&bpp, BPP); // We force RGBA color.
    if (!data)
    {
        fprintf(stderr, "Failed to load texture file (%s)!\n", file_name.c_str());
        return false;
    }
    defer { stbi_image_free(data); };

    texture.w     = w;
    texture.h     = h;
    texture.count = w/h;
    texture.size  = w/texture.count;

    if (w != (h * texture.count))
    {
        fprintf(stderr, "File (%s) must contain square textures packed horizontally!\n", file_name.c_str());
        return false;
    }

    texture.pixels = std::vector<u32>(w*h);
    for (int iy=0; iy<h; ++iy)
    {
        for (int ix=0; ix<w; ++ix)
        {
            u8 r = data[((iy*w+ix)*4)+0];
            u8 g = data[((iy*w+ix)*4)+1];
            u8 b = data[((iy*w+ix)*4)+2];
            u8 a = data[((iy*w+ix)*4)+3];
            texture.pixels[iy*w+ix] = pack_color(r,g,b,a);
        }
    }

    return true;
}

static void save_ppm (const std::string file_name, const Image& image)
{
    assert((image.w*image.h) == image.pixels.size());

    std::ofstream file(file_name, std::ios::binary);
    if (file.is_open())
    {
        file << "P6\n" << image.w << " " << image.h << "\n255\n";
        for (int i=0; i<image.pixels.size(); ++i)
        {
            u8 r,g,b,a;
            unpack_color(image.pixels[i], r,g,b,a);
            file << (u8)(r) << (u8)(g) << (u8)(b);
        }
        file.close();
    }
}

int main (int argc, char** argv)
{
    assert(sizeof(MAP) == ((MAP_W*MAP_H)+1)); // +1 for the null-terminator.

    // Setup our player character.
    float player_x = 3.456f;
    float player_y = 2.345f;
    float player_a = 1.523f;

    // Setup our framebuffer for drawing to.
    Image framebuffer;
    framebuffer.pixels.resize(WIN_W*WIN_H, pack_color(0xFF,0xFF,0xFF)); // Fill the framebuffer with white.
    framebuffer.w = WIN_W;
    framebuffer.h = WIN_H;

    // Load all of the wall textures.
    Texture walls;
    if (!load_texture("data/walls.png", walls))
    {
        fprintf(stderr, "Failed to load the wall textures!\n");
        return 0;
    }

    // Setup random colors for the tiles.
    constexpr int NUM_COLORS = 10;
    u32 colors[NUM_COLORS];
    for (int i=0; i<NUM_COLORS; ++i)
    {
        colors[i] = pack_color(rand()%0xFF, rand()%0xFF, rand()%0xFF);
    }

    // Draw tiles for each of the filled in spaces on the map.
    constexpr int RECT_W = ((WIN_W/2) / MAP_W);
    constexpr int RECT_H = ((WIN_H  ) / MAP_H);

    for (int iy=0; iy<MAP_H; ++iy)
    {
        for (int ix=0; ix<MAP_W; ++ix)
        {
            if (MAP[iy * MAP_W + ix] == ' ') continue;
            int color_index = MAP[iy * MAP_W + ix] - '0';
            assert(color_index < NUM_COLORS);
            int rx = ix*RECT_W, ry = iy*RECT_H;
            draw_rect(framebuffer, rx, ry, RECT_W, RECT_H, colors[color_index]);
        }
    }

    // Cast rays out for each vertical line of the viewport (WIN_W/2).
    for (int i=0; i<(WIN_W/2); ++i)
    {
        float angle = (player_a - (FOV/2.0f)) + (FOV * ((float)(i) / (float)(WIN_W/2)));
        for (float t=0.0f; t<20.0f; t+=0.01f)
        {
            float cx = player_x + t * cosf(angle);
            float cy = player_y + t * sinf(angle);

            float pix_x = cx * (float)(RECT_W);
            float pix_y = cy * (float)(RECT_H);

            // Draws the topdown view of the rays being fired out to the left viewport.
            framebuffer.pixels[((int)(pix_y)) * WIN_W + ((int)(pix_x))] = pack_color(0xA0,0xA0,0xA0);

            // When a ray hits a wall draw the 3D representation to the right viewport.
            if (MAP[((int)(cy)) * MAP_W + ((int)(cx))] != ' ')
            {
                int color_index = MAP[((int)(cy)) * MAP_W + ((int)(cx))] - '0';
                assert(color_index < NUM_COLORS);
                int height = (int)((float)(WIN_H) / (t*cosf(angle-player_a)));
                draw_rect(framebuffer, ((WIN_W/2)+i), ((WIN_H/2)-(height/2)), 1, height, colors[color_index]);
                break; // We don't need to continue casting the ray.
            }
        }
    }

    save_ppm("output.ppm", framebuffer);

    return 0;
}
