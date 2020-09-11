#pragma once

typedef std::vector<u32> TexCol;

struct Texture
{
    std::vector<u32> pixels;

    int w,h;
    int count; // Number of textures.
    int size;  // Size of textures.

    Texture (std::string file_name);

    u32&   get_pixel         (int x, int y, int id);
    TexCol get_scaled_column (int id, int coord, int height);
};
