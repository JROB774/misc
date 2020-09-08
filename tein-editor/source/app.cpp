namespace App
{
	GPU_Target* screen = nullptr;
	SDL_Event event;
	
	namespace
	{
		// makes identifying the two builds easier
		#ifdef RELEASE
		constexpr const char* WINDOW_CAPTION = "TEIN Editor ~ v";
		#else
		constexpr const char* WINDOW_CAPTION = "(DEBUG) TEIN Editor ~ v";
		#endif

		constexpr std::uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL;
		constexpr std::uint16_t WINDOW_MINWIDTH = 1280, WINDOW_MINHEIGHT = 720;

		SDL_Window* window = nullptr;
		std::string caption;
		std::uint16_t width = WINDOW_MINWIDTH, height = WINDOW_MINHEIGHT;
		bool running = false;

		SDL_Color clearcolor;

		int argc = 0;
		char** argv = nullptr;

		void(*update)() = nullptr;
		void(*render)() = nullptr;
		void(*handle)() = nullptr;
	}

	int GetCmdArgCount ()
	{
		return argc;
	}

	std::string GetCmdArg (int _index)
	{
		return ((_index >= argc) ? std::string() : std::string(argv[_index]));
	}

	void Init (int _argc, char** _argv)
	{
		// sets a custom termination function defined in error
		std::set_terminate(Error::Terminate);

		// sets the command line arg info for easy access
		argc = _argc;
		argv = _argv;

		// initialise SDL2 and extension libs (must be done before GPU)
		if (SDL_Init(SDL_INIT_VIDEO) != 0) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_SDL, "Failed to initialise SDL!"); }
		if (TTF_Init() != 0) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_TTF, "Failed to initialise TTF!"); }

		// creates a window to be used by GPU for rendering (allows for custom flags and caption)
		caption = WINDOW_CAPTION + Version::GetString();
		width = WINDOW_MINWIDTH, height = WINDOW_MINHEIGHT;
		window = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, WINDOW_FLAGS);
		if (!window) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_SDL, "Failed to create application window!"); }
		// extra settings regarding the underlying window
		SDL_SetWindowMinimumSize(window, WINDOW_MINWIDTH, WINDOW_MINHEIGHT);
		SDL_SetWindowIcon(window, nullptr);

		// initialise the GPU-accelerated renderer
		GPU_SetInitWindow(SDL_GetWindowID(window)); // sets the underlying window to use for init
		screen = GPU_InitRenderer(GPU_RENDERER_OPENGL_3, width, height, GPU_DEFAULT_INIT_FLAGS);
		if (!screen) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_GPU, "Failed to initialise GPU!"); }

		// platform-specific code initialised
		Platform::Init(window, argc, GetCmdArg(0));

		// initialises the application's subsystems
		DataManager::Init();
		Settings::Init();
		Cursor::Init();
		Asset::Init();

		// sets the default target that will be rendered to (aka. the screen)
		Render::SetDefaultTarget(screen);
		// sets the default clear color (background color)
		clearcolor = Settings::GetColor("background_color");
	}

	void Quit ()
	{
		// cleans up the application's subsystems
		Asset::Quit();
		Cursor::Quit();

		// free the window memory manually
		SDL_DestroyWindow(window);
		window = nullptr;

		// clean up SDL+GPU after use
		GPU_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	void SetCallbacks (void(*_update)(), void(*_render)(), void(*_handle)())
	{
		update = _update;
		render = _render;
		handle = _handle;
	}

	void Run ()
	{
		// do not run if callbacks aren't set
		if (!update || !render || !handle) {
			ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_STD, "No callbacks set!");
		}

		// the editor is now running
		running = true;
		// enter the main application loop
		while (running) {
			// handle all events
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					// handles window-based events
					case (SDL_WINDOWEVENT): {
						switch (event.window.event) {
							case (SDL_WINDOWEVENT_RESIZED):
							case (SDL_WINDOWEVENT_SIZE_CHANGED): {
								width = event.window.data1, height = event.window.data2;
								GPU_SetWindowResolution(width, height);
							} break;
						}
					} break;
				}
				// default handler for current scene
				handle();
			}
			// update and render
			update();
			GPU_ClearColor(screen, clearcolor);
			render();
			GPU_Flip(screen);
		}
	}

	void Stop ()
	{
		running = false;
	}

	void AppendCaption (std::string _text)
	{
		std::string newcaption = caption + _text;
		SDL_SetWindowTitle(window, newcaption.c_str());
	}

	std::uint16_t GetWidth ()
	{
		return width;
	}

	std::uint16_t GetHeight ()
	{
		return height;
	}

	SDL_Window* GetWindow ()
	{
		return window;
	}
}