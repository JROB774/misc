#pragma once

// INDIVIDUAL WINDOW FUNCTIONALITY

typedef void(*Window_Resize_Callback)(void);
typedef void(*Window_Close_Callback)(void);

typedef u32 Window_ID;

struct Window
{
    Window_Resize_Callback resize_callback;
    Window_Close_Callback close_callback;

    SDL_Window* window;
    Window_ID id;

    float width;
    float height;

    bool focus;
    bool mouse;
};

STDDEF bool create_window       (std::string name_, std::string title_, int x_, int y_, int w_, int h_, int mw_=0, int mh_=0, u32 flags_=0);
FILDEF bool is_window_hidden    (std::string name_);
FILDEF void show_window         (std::string name_);
FILDEF void hide_window         (std::string name_);
FILDEF void raise_window        (std::string name_);
FILDEF void set_window_title    (std::string name_, std::string title_);
FILDEF void set_window_icon     (std::string name_, std::string icon_);
FILDEF void set_window_min_size (std::string name_, int mw_, int mh_);
FILDEF void set_window_max_size (std::string name_, int mw_, int mh_);
FILDEF void set_window_pos      (std::string name_, int  x_, int  y_);
FILDEF void set_window_size     (std::string name_, int  w_, int  h_);

// GLOBAL WINDOW FUNCTIONALITY

GLOBAL constexpr const char* MAIN_WINDOW_TITLE  = "GHOSTS";
GLOBAL constexpr int         MAIN_WINDOW_X      = SDL_WINDOWPOS_CENTERED;
GLOBAL constexpr int         MAIN_WINDOW_Y      = SDL_WINDOWPOS_CENTERED;
GLOBAL constexpr int         MAIN_WINDOW_BASE_W = 1280;
GLOBAL constexpr int         MAIN_WINDOW_BASE_H =  720;
GLOBAL constexpr int         MAIN_WINDOW_MIN_W  =  800;
GLOBAL constexpr int         MAIN_WINDOW_MIN_H  =  600;
GLOBAL constexpr u32         MAIN_WINDOW_FLAGS  = SDL_WINDOW_RESIZABLE;

GLOBAL SDL_Event main_event;
GLOBAL bool      main_running;

FILDEF bool init_window (); // Creates the main window for us!
FILDEF void quit_window ();

FILDEF void handle_window_events ();

FILDEF Window&     get_window              (std::string name_);
FILDEF Window&     get_window_from_id      (Window_ID id_);
FILDEF std::string get_window_name_from_id (Window_ID id_);

FILDEF Window& get_focused_window ();
