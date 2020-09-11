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
#include "raycast.hpp"

#include "utils.cpp"
#include "framebuffer.cpp"
#include "map.cpp"
#include "player.cpp"
#include "texture.cpp"
#include "sprite.cpp"
#include "raycast.cpp"

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

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(fbuf.w, fbuf.h, SDL_WINDOW_SHOWN, &window, &renderer);

    assert(window);
    assert(renderer);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, fbuf.w, fbuf.h);
    assert(texture);
    SDL_UpdateTexture(texture, NULL, (void*)(fbuf.pixels.data()), fbuf.w*4);

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL,NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

