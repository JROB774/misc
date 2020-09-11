Framebuffer::Framebuffer (int fw, int fh)
{
    w = fw, h = fh;
    pixels.resize(w*h);
}

void Framebuffer::clear (u32 color)
{
    pixels = std::vector<u32>(w*h, color);
}

void Framebuffer::set_pixel (int px, int py, u32 color)
{
    assert((pixels.size() == (w*h)) && (px<w) && (py<h));
    pixels[py*w+px] = color;
}

void Framebuffer::draw_rect (int rx, int ry, int rw, int rh, u32 color)
{
    assert((pixels.size() == (w*h)));
    for (int iy=0; iy<rh; ++iy)
    {
        for (int ix=0; ix<rw; ++ix)
        {
            int px = rx + ix;
            int py = ry + iy;
            if (px < w && py < h) // No need to check for negatives (unsigned).
            {
                set_pixel(px,py, color);
            }
        }
    }
}
