#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

#include <SDL2/SDL.h>
#include <stb_image.h>
#include <defer.hpp>

#include "utils.hpp"
#include "framebuffer.hpp"
#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "sprite.hpp"

#include "utils.cpp"
#include "framebuffer.cpp"
#include "map.cpp"
#include "player.cpp"
#include "texture.cpp"
#include "sprite.cpp"

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

static void map_show_sprite (Framebuffer& fbuf, Map& map, Sprite& sprite)
{
    const int rw = (fbuf.w/2) / map.w;
    const int rh = (fbuf.h  ) / map.h;

    fbuf.draw_rect(sprite.x*rw-3, sprite.y*rh-3, 6, 6, pack_color(0xFF,0x00,0x00));
}

static void draw_sprite (Framebuffer& fbuf, std::vector<float>& dbuf, Player& player, Texture& tex, Sprite& sprite)
{
    // Absolute direction from the player to the sprite in radians.
    float sprite_dir = atan2(sprite.y - player.y, sprite.x - player.x);
    // Remove unncesessary periods from the relative direction.
    while ((sprite_dir - player.a) >  M_PI) sprite_dir -= 2 * M_PI;
    while ((sprite_dir - player.a) < -M_PI) sprite_dir += 2 * M_PI;

    // Screen sprite size.
    int sprite_screen_size = std::min(1000, (int)(fbuf.h/sprite.player_dist));

    int h_offset = (sprite_dir - player.a)/player.fov*(fbuf.w/2) + (fbuf.w/2)/2 - tex.size/2;
    int v_offset = fbuf.h/2 - sprite_screen_size/2;

    for (int ix=0; ix<sprite_screen_size; ++ix)
    {
        if (h_offset+((int)(ix))<0 || h_offset+ix>=fbuf.w/2) continue;
        if (dbuf[h_offset+ix] < sprite.player_dist) continue; // The sprite column is occluded.
        for (int iy=0; iy<sprite_screen_size; iy++)
        {
            if ((v_offset+((int)(iy))<0) || (v_offset+iy)>=fbuf.h) continue;

            u32 color = tex.get_pixel(ix*tex.size/sprite_screen_size, iy*tex.size/sprite_screen_size, sprite.texid);
            u8 r,g,b,a;
            unpack_color(color,r,g,b,a);
            if (a > 0x7F) {
                fbuf.set_pixel(fbuf.w/2 + h_offset+ix, v_offset+iy, color);
            }
        }
    }
}

static void render (Framebuffer& fbuf, Map& map, Player& player, Texture& walls, Texture& monsters, std::vector<Sprite>& sprites)
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
    std::vector<float> depthbuffer(fbuf.w/2, FLT_MAX);
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
            float dist = (t*cosf(angle-player.a));
            depthbuffer[i] = dist;
            int height = fbuf.h/dist;
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

    // Sort the sprites so they draw in the correct order.
    for (int i=0; i<sprites.size(); i++)
    {
        sprites[i].player_dist = sqrtf(powf(player.x - sprites[i].x, 2) + powf(player.y - sprites[i].y, 2));
    }
    std::sort(sprites.begin(), sprites.end()); // sort it from farthest to closest

    // Draw sprites.
    for (int i=0; i<sprites.size(); ++i) {
        map_show_sprite(fbuf, map, sprites[i]);
        draw_sprite(fbuf, depthbuffer, player, monsters, sprites[i]);
    }
}

int main (int argc, char** argv)
{
    Framebuffer fbuf(1024, 512);
    Player player(3.456f, 2.345f, 1.523f, M_PI/3.0f);
    Texture walls("data/walls.png");
    Texture monsters("data/sprites.png");
    Map map;

    assert(walls.count);
    assert(monsters.count);

    std::vector<Sprite> sprites { { 3.523f, 3.812f, 2, 0 }, { 1.834f, 8.765f, 0, 0 }, { 5.323f, 5.365f, 1, 0 }, { 4.123f, 10.265f, 1, 0 } };

    render(fbuf, map, player, walls, monsters, sprites);
    save_ppm("output.ppm", fbuf.pixels, fbuf.w, fbuf.h);

    return 0;
}
