static void unpack_color (u32 color, u8& r, u8& g, u8& b, u8& a)
{
    r = (color >>  0) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = (color >> 16) & 0xFF;
    a = (color >> 24) & 0xFF;
}

static u32 pack_color (u8 r, u8 g, u8 b, u8 a)
{
    return ((a << 24) + (b << 16) + (g << 8) + (r));
}

static void save_ppm (const std::string file_name, const std::vector<u32>& pixels, int w, int h)
{
    assert((w*h) == pixels.size());

    std::ofstream file(file_name, std::ios::binary);
    if (file.is_open())
    {
        file << "P6\n" << w << " " << h << "\n255\n";
        for (int i=0; i<pixels.size(); ++i)
        {
            u8 r,g,b,a;
            unpack_color(pixels[i], r,g,b,a);
            file << (u8)(r) << (u8)(g) << (u8)(b);
        }
        file.close();
    }
}
