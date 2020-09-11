#pragma once

struct Framebuffer
{
    std::vector<u32> pixels;
    int w,h;

    Framebuffer (int fw, int fh);

    void clear     (u32 color);
    void set_pixel (int px, int py, u32 color);
    void draw_rect (int rx, int ry, int rw, int rh, u32 color);
};
