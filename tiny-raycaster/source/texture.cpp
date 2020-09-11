Texture::Texture (std::string file_name)
{
    constexpr int BPP = 4;
    int bpp;
    u8* data = stbi_load(file_name.c_str(), &w,&h,&bpp, BPP); // We force RGBA color.
    if (!data)
    {
        fprintf(stderr, "Failed to load texture file (%s)!\n", file_name.c_str());
        return;
    }
    defer { stbi_image_free(data); };

    count = w/h;
    size  = w/count;

    if (w != (h * count))
    {
        fprintf(stderr, "File (%s) must contain square textures packed horizontally!\n", file_name.c_str());
        return;
    }

    pixels = std::vector<u32>(w*h);
    for (int iy=0; iy<h; ++iy)
    {
        for (int ix=0; ix<w; ++ix)
        {
            u8 r = data[((iy*w+ix)*4)+0];
            u8 g = data[((iy*w+ix)*4)+1];
            u8 b = data[((iy*w+ix)*4)+2];
            u8 a = data[((iy*w+ix)*4)+3];
            pixels[iy*w+ix] = pack_color(r,g,b,a);
        }
    }
}

u32& Texture::get_pixel (int x, int y, int id)
{
    assert((x<size) && (y<size) && (id<count));
    return pixels[y*w+(x+(id*size))];
}

TexCol Texture::get_scaled_column (int id, int coord, int height)
{
    assert((coord<size) && (id<count));
    std::vector<u32> column(height);
    for (int y=0; y<height; y++)
    {
        column[y] = get_pixel(coord, (y*size)/height, id);
    }
    return column;
}
