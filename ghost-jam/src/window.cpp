namespace internal
{
    GLOBAL std::map<std::string, Window> windows;

    FILDEF bool is_subwindow_open ()
    {
        for (auto it: windows) {
            if (it.first != "WINMAIN") {
                if (!is_window_hidden(it.first)) {
                    return true;
                }
            }
        }
        return false;
    }
}

STDDEF bool create_window (std::string name_, std::string title_, int x_, int y_, int w_, int h_, int mw_, int mh_, u32 flags_)
{
    if (internal::windows.find(name_) != internal::windows.end()) {
        LOG_ERROR(ERR_MAX, "Window with name \"%s\" already exists!", name_.c_str());
        return false;
    }

    internal::windows.insert(std::pair<std::string, Window>(name_, Window()));
    Window& window = internal::windows.at(name_);

    flags_ |= (SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_OPENGL); // Required flags!
    window.window = SDL_CreateWindow(title_.c_str(), x_, y_, w_, h_, flags_);
    if (!window.window) {
        LOG_ERROR(ERR_MIN, "Failed to create window! (%s)", SDL_GetError());
        return false;
    }

    window.id = SDL_GetWindowID(window.window);
    if (!window.id) {
        LOG_ERROR(ERR_MIN, "Failed to get window ID! (%s)", SDL_GetError());
        return false;
    }

    // Only set a minimum size if both values are a valid size.
    if (mw_ && mh_) { set_window_min_size(name_, mw_, mh_); }

    // We use SDL_GetWindowSize to cache the final size of a window in case
    // any flags such as SDL_WINDOW_MAXIMIZED were used on window creation.
    int final_width;
    int final_height;

    SDL_GetWindowSize(window.window, &final_width, &final_height);

    window.width  = CAST(float, final_width);
    window.height = CAST(float, final_height);

    // Default to false and it will get handled by the window event system.
    window.focus = false;
    window.mouse = false;

    LOG_DEBUG("Created Window %s", name_.c_str());
    return true;
}

FILDEF bool is_window_hidden (std::string name_)
{
    return (SDL_GetWindowFlags(internal::windows.at(name_).window) & SDL_WINDOW_HIDDEN);
}

FILDEF void show_window (std::string name_)
{
    SDL_ShowWindow(internal::windows.at(name_).window);
}

FILDEF void hide_window (std::string name_)
{
    SDL_HideWindow(internal::windows.at(name_).window);
}

FILDEF void raise_window (std::string name_)
{
    SDL_RaiseWindow(internal::windows.at(name_).window);
}

FILDEF void set_window_title (std::string name_, std::string title_)
{
    SDL_SetWindowTitle(internal::windows.at(name_).window, title_.c_str());
}

FILDEF void set_window_icon (std::string name_, std::string icon_)
{
    int w, h, bpp;
    u8* raw_data = stbi_load(Catalog::build_resource_path("ICONS/" + icon_ + ".png").c_str(), &w,&h,&bpp, 4);
    if (!raw_data) {
        LOG_ERROR(ERR_MIN, "Failed to load window icon!");
        return;
    }
    defer { stbi_image_free(raw_data); };

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(raw_data, w, h, bpp*8, w*bpp, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        LOG_ERROR(ERR_MIN, "Failed to create window icon! (%s)", SDL_GetError());
        return;
    }

    SDL_SetWindowIcon(internal::windows.at(name_).window, surface);
    SDL_FreeSurface(surface);
}

FILDEF void set_window_min_size (std::string name_, int mw_, int mh_)
{
    SDL_SetWindowMinimumSize(internal::windows.at(name_).window, mw_, mh_);
}

FILDEF void set_window_max_size (std::string name_, int mw_, int mh_)
{
    SDL_SetWindowMaximumSize(internal::windows.at(name_).window, mw_, mh_);
}

FILDEF void set_window_pos (std::string name_, int x_, int y_)
{
    SDL_SetWindowPosition(internal::windows.at(name_).window, x_, y_);
}

FILDEF void set_window_size (std::string name_, int w_, int h_)
{
    SDL_SetWindowSize(internal::windows.at(name_).window, w_, h_);
}

FILDEF bool init_window ()
{
    // The SDL docs say that this should be done before creation of the window!
    int profile_mask =  SDL_GL_CONTEXT_PROFILE_CORE;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_CONTEXT_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_CONTEXT_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,                                 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,              profile_mask);

    #if defined(DEBUG_BUILD)
    std::string main_title(format_string("[DEBUG] %s (%d.%d.%d)", MAIN_WINDOW_TITLE, GAME_VERSION_MAJOR,GAME_VERSION_MINOR,GAME_VERSION_PATCH));
    #else
    std::string main_title(format_string("%s (%d.%d.%d)", MAIN_WINDOW_TITLE, GAME_VERSION_MAJOR,GAME_VERSION_MINOR,GAME_VERSION_PATCH));
    #endif

    if (!create_window("WINMAIN", main_title.c_str(), MAIN_WINDOW_X,MAIN_WINDOW_Y,MAIN_WINDOW_BASE_W,MAIN_WINDOW_BASE_H, MAIN_WINDOW_MIN_W,MAIN_WINDOW_MIN_H, MAIN_WINDOW_FLAGS)) {
        LOG_ERROR(ERR_MAX, "Failed to create the main application window!");
        return false;
    }

    get_window("WINMAIN").close_callback = []() {
        main_running = false;
    };

    return true;
}

FILDEF void quit_window ()
{
    for (auto it: internal::windows) {
        SDL_DestroyWindow(it.second.window);
    }
    internal::windows.clear();
}

FILDEF void handle_window_events ()
{
    if (main_event.type != SDL_WINDOWEVENT) { return; }

    std::string window_name = get_window_name_from_id(main_event.window.windowID);
    Window& window = get_window_from_id(main_event.window.windowID);

    switch (main_event.window.event) {
    case (SDL_WINDOWEVENT_FOCUS_GAINED): {
        // Special case for the main window to ensure all sub-windows stay on top of it.
        if (window_name == "WINMAIN") {
            if (internal::is_subwindow_open()) {
                for (auto it: internal::windows) {
                    if (it.first != "WINMAIN") {
                        raise_window(it.first);
                    }
                }
            } else {
                window.focus = true;
            }
        } else {
            window.focus = true;
        }
    } break; // @Incomplete: Implement sub-window priority (see TEIN Editor).
    case (SDL_WINDOWEVENT_FOCUS_LOST): { window.focus = false; } break;

    case (SDL_WINDOWEVENT_ENTER): { window.mouse = true; } break;
    case (SDL_WINDOWEVENT_LEAVE): { window.mouse = false; } break;

    case (SDL_WINDOWEVENT_CLOSE): {
        hide_window(window_name);
        if (window.close_callback) {
            window.close_callback();
        }
    } break;

    case (SDL_WINDOWEVENT_SIZE_CHANGED):
    case (SDL_WINDOWEVENT_RESIZED): {
        window.width = CAST(float, main_event.window.data1);
        window.height = CAST(float, main_event.window.data2);
        if (window.resize_callback) {
            window.resize_callback();
        }
    } break;
    }
}

FILDEF Window& get_window (std::string name_)
{
    ASSERT(internal::windows.find(name_) != internal::windows.end());
    return internal::windows.at(name_);
}
FILDEF Window& get_window_from_id (Window_ID id_)
{
    for (auto& it: internal::windows) {
        if (it.second.id == id_) {
            return it.second;
        }
    }
    ASSERT(false);
    return internal::windows.at("");
}
FILDEF std::string get_window_name_from_id (Window_ID id_)
{
    for (auto& it: internal::windows) {
        if (it.second.id == id_) {
            return it.first;
        }
    }
    ASSERT(false);
    return std::string();
}

FILDEF Window& get_focused_window ()
{
    for (auto& it: internal::windows) {
        if (it.second.focus) {
            return it.second;
        }
    }
    ASSERT(false);
    return internal::windows.at("WINMAIN");
}
