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

#include "utils.hpp"
#include "framebuffer.hpp"
#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"

#include "utils.cpp"
#include "framebuffer.cpp"
#include "map.cpp"
#include "player.cpp"
#include "texture.cpp"

static int wall_x_texcoord (float x, float y, Texture& walls)
{
    // These contain fractional parts of cx and cy and vary between [-0.5 to +0.5] and one of them will be close to zero.
    float hitx = x - floorf(x + 0.5f);
    float hity = y - floorf(y + 0.5f);

    // Determine if we hit vertical or horizontal.
    int tex = ((abs(hity) > abs(hitx)) ? (int)(hity*walls.size) : (int)(hitx*walls.size));
    if (tex < 0) tex += walls.size; // Coordinate can be negative, so we need to fix it.

    assert((tex>=0) && (tex<walls.size));

    return tex;
}

static void render (Framebuffer& fbuf, Map& map, Player& player, Texture& walls)
{
    fbuf.clear(pack_color(0xFF,0xFF,0xFF));

    const int rw = (fbuf.w/2) / map.w;
    const int rh = (fbuf.h  ) / map.h;

    // Draw the tile map.
    for (int iy=0; iy<map.h; ++iy)
    {
        for (int ix=0; ix<map.w; ++ix)
        {
            if (map.is_empty(ix,iy)) continue; // Skip empties...

            int rx = ix * rw;
            int ry = iy * rh;

            int id = map.get_tile(ix,iy);
            assert(id<walls.count);

            fbuf.draw_rect(rx,ry,rw,rh, walls.get_pixel(0,0,id));
        }
    }

    // Ray marching loop.
    for (int i=0; i<fbuf.w/2; ++i)
    {
        float angle = (player.a-(player.fov/2)) + (player.fov*(i/(float)(fbuf.w/2)));
        for (float t=0.0f; t<20.0f; t+=0.01f)
        {
            float x = player.x + t * cosf(angle);
            float y = player.y + t * sinf(angle);

            // Draw the visibility cone.
            fbuf.set_pixel(x*rw, y*rh, pack_color(0xA0,0xA0,0xA0));

            if (map.is_empty(x,y)) continue;

            // Draw the 3D perspective scene.
            int id = map.get_tile(x,y);
            assert(id<walls.count);
            int height = fbuf.h/(t*cosf(angle-player.a));
            int coord = wall_x_texcoord(x,y,walls);
            auto column = walls.get_scaled_column(id,coord,height);
            int px = i+(fbuf.w/2);
            for (int j=0; j<height; ++j)
            {
                int py = j+(fbuf.h/2)-(height/2);
                if ((py >= 0) && (py < (int)fbuf.h))
                {
                    fbuf.set_pixel(px,py,column[j]);
                }
            }

            break;
        }
    }
}

int main (int argc, char** argv)
{
    Framebuffer fbuf(1024, 512);
    Player player(3.456f, 2.345f, 1.523f, M_PI/3.0f);
    Texture walls("data/walls.png");
    Map map;

    assert(walls.count);

    render(fbuf, map, player, walls);
    save_ppm("output.ppm", fbuf.pixels, fbuf.w, fbuf.h);

    return 0;
}
