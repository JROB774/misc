#include <cstdint>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef   int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;

static constexpr int WIN_W = 512;
static constexpr int WIN_H = 512;
static constexpr int MAP_W =  16;
static constexpr int MAP_H =  16;

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

struct Image
{
    std::vector<u32> pixels;
    int w, h;
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
            assert((cx < image.w) && (cy < image.h));
            image.pixels[cy * image.w + cx] = color;
        }
    }
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

    float player_x = 3.456f;
    float player_y = 2.345f;
    float player_a = 1.523f;

    Image framebuffer;

    framebuffer.pixels.resize(WIN_W*WIN_H, 0x000000FF); // Fill the framebuffer with red.

    framebuffer.w = WIN_W;
    framebuffer.h = WIN_H;

    // Draw a smooth gradient across the whole of the framebuffer.
    for (int iy=0; iy<framebuffer.h; ++iy)
    {
        for (int ix=0; ix<framebuffer.w; ++ix)
        {
            u8 r = (u8)(0xFF * iy / framebuffer.h);
            u8 g = (u8)(0xFF * ix / framebuffer.w);
            u8 b = (u8)(0x00);

            framebuffer.pixels[iy * framebuffer.w + ix] = pack_color(r,g,b);
        }
    }

    // Draw tiles for each of the filled in spaces on the map.
    constexpr int RECT_W = WIN_W / MAP_W;
    constexpr int RECT_H = WIN_H / MAP_H;

    for (int iy=0; iy<MAP_H; ++iy)
    {
        for (int ix=0; ix<MAP_W; ++ix)
        {
            if (MAP[iy * MAP_W + ix] == ' ') continue;
            int rx = ix*RECT_W, ry = iy*RECT_H;
            draw_rect(framebuffer, rx, ry, RECT_W, RECT_H, pack_color(0x00,0xFF,0xFF));
        }
    }

    // Draw the player on the map.
    float px = player_x * (float)(RECT_W);
    float py = player_y * (float)(RECT_H);

    draw_rect(framebuffer, (int)(px), (int)(py), 5,5, pack_color(0xFF,0xFF,0xFF));

    // Cast a single ray out from the player's view direction.
    for (float t=0.0f; t<20.0f; t+=0.05f)
    {
        float cx = player_x + t * cosf(player_a);
        float cy = player_y + t * sinf(player_a);

        if (MAP[((int)(cy)) * MAP_W + ((int)(cx))] != ' ') break;

        float pix_x = cx * (float)(RECT_W);
        float pix_y = cy * (float)(RECT_H);

        framebuffer.pixels[((int)(pix_y)) * WIN_W + ((int)(pix_x))] = pack_color(0xFF,0xFF,0xFF);
    }

    save_ppm("output.ppm", framebuffer);

    return 0;
}
