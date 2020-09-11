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
            file << static_cast<u8>(r) << static_cast<u8>(g) << static_cast<u8>(b);
        }
        file.close();
    }
}

int main (int argc, char** argv)
{
    Image framebuffer;

    framebuffer.pixels.resize(WIN_W*WIN_H, 0x000000FF); // Fill the framebuffer with red.

    framebuffer.w = WIN_W;
    framebuffer.h = WIN_H;

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

    save_ppm("output.ppm", framebuffer);

    return 0;
}
