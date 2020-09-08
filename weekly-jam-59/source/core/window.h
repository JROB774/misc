/*******************************************************************************
 * WINDOW
 * The main window and renderer for the application implemented using SDL2.
 *
*******************************************************************************/

#ifndef __TCE_WINDOW_H__ /*///////////////////////////////////////////////////*/
#define __TCE_WINDOW_H__

namespace TCE
{

GLOBAL constexpr const char* WINDOW_TITLE       = "Team Chromo Engine";
GLOBAL constexpr int         WINDOW_X           = SDL_WINDOWPOS_CENTERED;
GLOBAL constexpr int         WINDOW_Y           = SDL_WINDOWPOS_CENTERED;
GLOBAL constexpr u32         WINDOW_FLAGS       = SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE;

// These need default values in case the variable tweak value cannot be loaded!
GLOBAL String window_title = "Team Chromo Engine";

GLOBAL int window_start_width = 800;
GLOBAL int window_start_height = 600;
GLOBAL int viewport_width = 800;
GLOBAL int viewport_height = 600;

GLOBAL SDL_Color border_clear_color   = {   0,   0,   0, 255 };
GLOBAL SDL_Color viewport_clear_color = { 255,   0, 255, 255 };

GLOBAL SDL_Window* window;
GLOBAL SDL_Rect viewport;

GLOBAL SDL_Renderer* renderer;
GLOBAL SDL_Texture* screen;
GLOBAL int render_scale;

GLOBAL SDL_Event event;

GLOBAL bool window_ready = false;
GLOBAL bool running = false;

INLDEF void WindowInit ();
INLDEF void WindowQuit ();

STDDEF void ResizeWindowViewport (int _new_window_width, int _new_window_height);
INLDEF void UpdateWindowValues ();

INLDEF void SetRendererColor (u8 _r, u8 _g, u8 _b, u8 _a);
INLDEF void SetRendererColor (const SDL_Color& _color);

INLDEF void SetRendererTarget (SDL_Texture* _texture);
INLDEF void SetRendererViewport (SDL_Rect* _viewport);
INLDEF void SetRendererScale (float _scale_x, float _scale_y);

INLDEF void RenderScreen ();
INLDEF void RenderClear ();

} // TCE

#endif /* __TCE_WINDOW_H__ ///////////////////////////////////////////////////*/
