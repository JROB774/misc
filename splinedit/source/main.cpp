#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

// This is only needed for Array because it may contain types (such as
// String) that need their constructor and destructor called to ensure
// they are created correctly and memory is deallocated afterwards.
//
// The overhead would be needed anyway for these types, so using new and
// delete in this instance (a generic container type) is fine. (Everywhere
// else uses malloc() and free() for memory management).
#include <new>

#include <SDL2/SDL.h>

#include "utility/utility.h"
#include "utility/utility.cpp"

GLOBAL constexpr const char* WINDOW_TITLE         = "Splinedit";
GLOBAL constexpr int         WINDOW_X             = SDL_WINDOWPOS_CENTERED;
GLOBAL constexpr int         WINDOW_Y             = SDL_WINDOWPOS_CENTERED;
GLOBAL constexpr int         WINDOW_MIN_WIDTH     = 800;
GLOBAL constexpr int         WINDOW_MIN_HEIGHT    = 600;
GLOBAL constexpr float       WINDOW_SCALE         = 4.0f;
GLOBAL constexpr int         WINDOW_WIDTH         = 800;
GLOBAL constexpr int         WINDOW_HEIGHT        = 600;
GLOBAL constexpr int         WINDOW_SCREEN_WIDTH  = (int)((float)WINDOW_WIDTH / WINDOW_SCALE);
GLOBAL constexpr int         WINDOW_SCREEN_HEIGHT = (int)((float)WINDOW_HEIGHT / WINDOW_SCALE);
GLOBAL constexpr u32         WINDOW_FLAGS         = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE;

GLOBAL SDL_Window* window;
GLOBAL SDL_Renderer* renderer;
GLOBAL SDL_Event event;
GLOBAL bool running;

#include "utility/array.h"
#include "render/render.h"
#include "core/cursor.h"
#include "core/spline.h"

#include "utility/array.cpp"
#include "render/render.cpp"
#include "core/cursor.cpp"
#include "core/spline.cpp"

int main (int _argc, char* _argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_X,WINDOW_Y, WINDOW_WIDTH,WINDOW_HEIGHT, WINDOW_FLAGS);
	SDL_SetWindowMinimumSize(window, WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetScale(renderer, WINDOW_SCALE, WINDOW_SCALE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Spline spline;

	running = true;
	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				// Handle modifying the subdivision amount.
				case (SDL_KEYUP): {
					if (event.key.keysym.sym == SDLK_LEFT) {
						spline_subdivide_step *= 2.0f;
						UpdateSplineShape(spline);
					} else if (event.key.keysym.sym == SDLK_RIGHT) {
						spline_subdivide_step /= 2.0f;
						UpdateSplineShape(spline);
					}
				} break;
				// Handle closing the application.
				case (SDL_QUIT): {
					running = false;
				} break;
				// Handle the user dragging spline nodes.
				default: {
					HandleSplineEvents(spline);
				} break;
			}
		}

		// Updates the composition of the spline.
		relative_cursor = GetRelativeScaledCursorPosition();
		UpdateSpline(spline);

		// Render the spline to the screen.
		SetRenderColor(DARK_GREY);
		SDL_RenderClear(renderer);
		RenderSpline(spline);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
