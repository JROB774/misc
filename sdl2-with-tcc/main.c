// These defines are necessary in order to get SDL2 working with TCC. We also had to go in
// to the SDL_config.h header file and change the type definitions of size_t and uintptr_t.
#define DECLSPEC
#define SDLCALL
#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"

int main (int argc, char** argv)
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_bool running;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(1280,720, SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "SDL2 TCC Test");

    running = SDL_TRUE;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = SDL_FALSE;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF,0x00,0x00,0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
