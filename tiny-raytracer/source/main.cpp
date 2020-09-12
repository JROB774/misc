#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cassert>

#include <fstream>
#include <vector>

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef   int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;

struct vec3 { float r,g,b; };

static void save_ppm (const std::string file_name, const std::vector<vec3>& pixels, int w, int h)
{
    assert((w*h) == pixels.size());

    std::ofstream file(file_name, std::ios::binary);
    if (file.is_open())
    {
        file << "P6\n" << w << " " << h << "\n255\n";
        for (int i=0; i<pixels.size(); ++i)
        {
            file << (u8)(0xFF * std::max(0.0f, std::min(1.0f, pixels[i].r)));
            file << (u8)(0xFF * std::max(0.0f, std::min(1.0f, pixels[i].g)));
            file << (u8)(0xFF * std::max(0.0f, std::min(1.0f, pixels[i].b)));
        }
        file.close();
    }
}

static void render ()
{
    constexpr int WIDTH  = 1024;
    constexpr int HEIGHT = 768;

    std::vector<vec3> framebuffer(WIDTH*HEIGHT);

    // Fill our framebuffer with a smooth gradient.
    for (int iy=0; iy<HEIGHT; ++iy)
    {
        for (int ix=0; ix<WIDTH; ++ix)
        {
            framebuffer[iy*WIDTH+ix] = vec3 { iy/(float)HEIGHT, ix/(float)WIDTH, 0.0f };
        }
    }

    // Save the framebuffer output to disk.
    save_ppm("output.ppm", framebuffer, WIDTH,HEIGHT);
}

int main (int argc, char** argv)
{
    render();
    return 0;
}
