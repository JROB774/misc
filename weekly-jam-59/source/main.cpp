/*////////////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

// This is only needed for Array because it may contain types (such as
// String) that need their constructor and destructor called to ensure
// they are created correctly and memory is deallocated afterwards.
//
// The overhead would be needed anyway for these types, so using new and
// delete in this instance (a generic container type) is fine. (Everywhere
// else uses malloc() and free() for memory management).
#include <new>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

// We don't care about external library warnings as we can't fix them anyway.
#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "external/Lua/lua.hpp"
#include "external/stb/stb_image.h"
#include "external/SDL2/SDL.h"
#include "external/SDL2/SDL_syswm.h"
#include "external/SDL2/SDL_mixer.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "utility/utility.h"
#include "utility/array.h"
#include "utility/string.h"
#include "utility/vector2d.h"
#include "platform/platform.h"
#include "log/error.h"
#include "log/debug.h"
#include "core/window.h"
#include "core/game.h"
#include "core/variables.h"
#include "core/console.h"
#include "core/event.h"
#include "core/collision.h"
#include "audio/mixer.h"
#include "audio/sound.h"
#include "audio/music.h"
#include "render/primitive.h"
#include "render/animation.h"
#include "render/texture.h"
#include "render/font.h"
#include "lua/lua_error.h"
#include "lua/export.h"
#include "lua/main_script.h"
#include "game/zone.h"
#include "game/lighting.h"
#include "game/pathfind.h"

#include "utility/utility.cpp"
#include "utility/array.cpp"
#include "utility/string.cpp"
#include "utility/vector2d.cpp"
#include "platform/platform.cpp"
#include "log/error.cpp"
#include "log/debug.cpp"
#include "core/window.cpp"
#include "core/game.cpp"
#include "core/variables.cpp"
#include "core/console.cpp"
#include "core/event.cpp"
#include "core/collision.cpp"
#include "audio/mixer.cpp"
#include "audio/sound.cpp"
#include "audio/music.cpp"
#include "render/primitive.cpp"
#include "render/animation.cpp"
#include "render/texture.cpp"
#include "render/font.cpp"
#include "lua/lua_error.cpp"
#include "lua/export.cpp"
#include "lua/main_script.cpp"
#include "game/zone.cpp"
#include "game/lighting.cpp"
#include "game/pathfind.cpp"

int main (int _argc, char* _argv[])
{
	using namespace TCE;

	// Set this here so fatal initialization errors can unset it.
	running = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MAX, ERROR_TYPE_SDL, "Failed to initialize SDL!");
		return 0;
	}
	SDL_StartTextInput();

	ErrorInit();
	DebugInit();

	VariablesInit();

	// We register and load all the variables at the start because some
	// may be needed for start-up, so we need to be assured that none of
	// the variables are yet to be registered and can be loaded safely.
	RegisterAllVariables();
	LoadAllVariables();

	HotloadInit();

	// Because there is only one variables file we do not need a handle to let the hotloader
	// know where it can find the file. It will simply call the LoadAllVariables function.
	RegisterAssetForHotloading(current_variables_path.c_string, ASSET_VARIABLES, INVALID_ASSET_HANDLE);

	WindowInit();
	PlatformInit();
	MixerInit();
	LightingInit();

	LuaMainInit();
	LuaGameInit();

	ConsoleInit();

	SetupGameTime();

	while (running) {
		// We update the time step every cycle in case a new refresh rate gets hotloaded.
		if (hotloader_enabled) {
			HotloadChangedAsset(); // Reload an asset if it has been changed.
			fixed_time_step = 1.0f / refresh_rate;
		}
		// If we want to reload the Lua state do it here where it's safe.
		if (reload_lua_state) {
			ReloadLuaMainScript();
		}

		// Handle any events passed by SDL for the engine.
		ClearEventState();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case (SDL_WINDOWEVENT): {
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						int new_width = event.window.data1;
						int new_height = event.window.data2;
						ResizeWindowViewport(new_width, new_height);
					}
				} break;
				case (SDL_QUIT): {
					running = false;
				} break;
			}
			// Fill the current frame's event state.
			FillEventState();
			// Handle input for the debug console.
			ConsoleHandleTextInput();
		}

		// All the game content should be drawn to the screen buffer.
		SetRendererTarget(screen);
		UpdateGame(delta_time);
		RenderGame();
		SetRendererTarget(NULL);

		// We then render the scaled screen and non-scaled elements.
		SetRendererViewport(&viewport);
		RenderScreen();
		ConsoleUpdateAndRender(delta_time);
		SetRendererViewport(NULL);

		UpdateTimeAndFlipBuffers();
	}

	// We only free these because their elements allocate dynamic memory.
	FreeTextureList();
	FreeSoundList();
	FreeMusic();

	LuaMainQuit();

	LightingQuit();
	MixerQuit();
	HotloadQuit();
	WindowQuit();

	DebugQuit();
	ErrorQuit();

	SDL_StopTextInput();
	SDL_Quit();

	return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/
