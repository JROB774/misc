/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void CreateScreenBuffer ()
{
	if (screen) { SDL_DestroyTexture(screen); screen = NULL; }

	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, viewport_width, viewport_height);
	if (!screen) {
		TCE_ERROR_LOG(ERROR_LEVEL_MAX, ERROR_TYPE_SDL, "Failed to create the screen buffer!");
	}
}

INLDEF void WindowInit ()
{
	window = SDL_CreateWindow(window_title.c_string, WINDOW_X, WINDOW_Y, window_start_width, window_start_height, WINDOW_FLAGS);
	if (!window) {
		TCE_ERROR_LOG(ERROR_LEVEL_MAX, ERROR_TYPE_SDL, "Failed to create window!");
		return;
	}
	SDL_SetWindowMinimumSize(window, viewport_width, viewport_height);
	SDL_SetWindowIcon(window, NULL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		TCE_ERROR_LOG(ERROR_LEVEL_MAX, ERROR_TYPE_SDL, "Failed to create renderer!");
		return;
	}
	// This allows for semi-transparent rendering of primitives, but is not essential.
	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to set renderer blend mode!");
	}

	ResizeWindowViewport(window_start_width, window_start_height);

	// This is the screen/buffer we draw to that then gets scaled up to the actual size.
	// Whereas elements such as the console can be drawn always at proper window res.
	CreateScreenBuffer();

	window_ready = true;
}

INLDEF void WindowQuit ()
{
	window_ready = false;

	SDL_DestroyTexture(screen);
	screen = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
}

STDDEF void ResizeWindowViewport (int _window_width, int _window_height)
{
	// Determine how much we can scale up the base viewport size by.
	render_scale = 1;
	while ((viewport_width * (render_scale+1)) <= _window_width &&
		   (viewport_height * (render_scale+1)) <= _window_height) {
		++render_scale;
	}

	// Center the viewport in the middle of the window.
	viewport.w = viewport_width * render_scale;
	viewport.h = viewport_height * render_scale;
	viewport.x = ((_window_width - viewport.w) / 2);
	viewport.y = ((_window_height - viewport.h) / 2);
}

INLDEF void UpdateWindowValues ()
{
	// If the window doesn't exist yet then we can't set these values.
	if (!window || !window_ready) { return; }

	int window_width, window_height;
	SDL_GetWindowSize(window, &window_width, &window_height);
	ResizeWindowViewport(window_width, window_height);

	SDL_SetWindowMinimumSize(window, viewport_width, viewport_height);
	SDL_SetWindowTitle(window, window_title.c_string);

	// Recreate the screen buffer that we draw to.
	CreateScreenBuffer();
}

INLDEF void SetRendererColor (u8 _r, u8 _g, u8 _b, u8 _a)
{
	SDL_Color renderer_color = { _r, _g, _b, _a };
	SetRendererColor(renderer_color);
}
INLDEF void SetRendererColor (const SDL_Color& _color)
{
	if (SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to set renderer color!");
	}
}

INLDEF void SetRendererTarget (SDL_Texture* _texture)
{
	if (SDL_SetRenderTarget(renderer, _texture) != 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to set renderer target!");
	}
}
INLDEF void SetRendererViewport (SDL_Rect* _viewport)
{
	if (SDL_RenderSetViewport(renderer, _viewport) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_SDL, "Failed to set the renderer viewport!");
	}
}
INLDEF void SetRendererScale (float _scale_x, float _scale_y)
{
	if (SDL_RenderSetScale(renderer, _scale_x, _scale_y) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to set the render scale!");
	}
}

INLDEF void RenderScreen ()
{
	SetRendererScale(CAST(float, render_scale), CAST(float, render_scale));
	SDL_Rect destination = { 0, 0, viewport_width, viewport_height };
	if (SDL_RenderCopy(renderer, screen, NULL, &destination) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to render the screen!");
	}
	SetRendererScale(1.0f, 1.0f);
}
INLDEF void RenderClear ()
{
	// Clear the entire window then clear just the viewport to a different color.
	SetRendererColor(border_clear_color);
	SDL_RenderClear(renderer);

	// Because render clear ignores the current viewport and clip we will
	// just have to draw a rectangle to fill the viewport with a different color.
	RenderRectFilled(0, 0, viewport.w, viewport.h, viewport_clear_color);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
