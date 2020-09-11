#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <thread>
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

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(fbuf.w, fbuf.h, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, fbuf.w, fbuf.h);

    assert(window);
    assert(renderer);
    assert(texture);

    bool running = true;
    SDL_Event event;

    auto t1 = std::chrono::high_resolution_clock::now();
    while (running)
    {
        // Sleep for a small period of time.
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        if (fp_ms.count() < 20)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            continue;
        }
        t1 = t2;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case (SDL_KEYDOWN):
                {
                    switch (event.key.keysym.sym)
                    {
                        case (SDLK_a): player.turn = -1; break;
                        case (SDLK_d): player.turn =  1; break;
                        case (SDLK_w): player.walk =  1; break;
                        case (SDLK_s): player.walk = -1; break;
                    }
                } break;
                case (SDL_KEYUP):
                {
                    switch (event.key.keysym.sym)
                    {
                        case (SDLK_a): case (SDLK_d): player.turn = 0; break;
                        case (SDLK_w): case (SDLK_s): player.walk = 0; break;
                    }
                } break;
                case (SDL_QUIT):
                {
                    running = false;
                } break;
            }
        }

        // Turn the player.
        player.a += (float)(player.turn) * 0.05f;

        // Move the player.
        float nx = player.x + player.walk * cosf(player.a) * 0.05f;
        float ny = player.y + player.walk * sinf(player.a) * 0.05f;
        if ((((int)(nx))>=0) && (((int)(nx))<((int)(map.w))) && (((int)(ny))>=0) && (((int)(ny))<((int)(map.h))))
        {
            if (map.is_empty(nx, player.y)) player.x = nx;
            if (map.is_empty(player.x, ny)) player.y = ny;
        }

        // Update sprite distances.
        for (int i=0; i<sprites.size(); i++)
        {
            sprites[i].player_dist = sqrtf(powf(player.x - sprites[i].x, 2) + powf(player.y - sprites[i].y, 2));
        }
        std::sort(sprites.begin(), sprites.end());

        // Render the scene.
        render(fbuf, map, player, walls, monsters, sprites);

        SDL_UpdateTexture(texture, NULL, (void*)(fbuf.pixels.data()), fbuf.w*4);
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

