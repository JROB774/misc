// c library includes
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <cctype>
#include <ctime>

// @TODO: GET RID OF ALL OF THESE
// c++ library includes
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <type_traits>
#include <valarray>
#include <utility>
#include <memory>
#include <exception>

// external library includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_gpu.h>
#include <SDL2/SDL_ttf.h>

// @TODO: GET RID OF THIS DEPENDENCY
// external headers
#include "extern/SDL_FontCache.h"

// these are set to avoid problems with certain source files needing
// functions/types from other source files, and vice-versa

// local headers
#include "include/platform.h"

// these are included to create one single translation unit, which
// compiles much faster than separate ones + removes need for headers

// local sources
#include "utility.cpp"
#include "version.cpp"
#include "error.cpp"
#include "platform.cpp"
#include "gpak.cpp"
#include "lvl.cpp"
#include "render.cpp"
#include "data.cpp"
#include "datamngr.cpp"
#include "settings.cpp"
#include "cursor.cpp"
#include "assets.cpp"
#include "app.cpp"
#include "hotkey.cpp"
#include "control.cpp"
#include "tooltip.cpp"
#include "hotbar.cpp"
#include "tiletable.cpp"
#include "tilepanel.cpp"
#include "lvleditor.cpp"

int main (int _argc, char* _argv[])
{
	// init the app and load the editor environment
	App::Init(_argc, _argv);
	LevelEditor::Init();
	App::SetCallbacks(LevelEditor::Update, LevelEditor::Render, LevelEditor::Handle);

	// run the app until the end of the program
	App::Run();

	// gracefully quit on exit
	LevelEditor::Quit();
	App::Quit();

	return 0;
}