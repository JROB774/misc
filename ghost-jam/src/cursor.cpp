namespace internal
{
    GLOBAL std::string current_cursor;
    GLOBAL std::map<std::string, SDL_Cursor*> cursors;

    FILDEF void load_cursor (std::string _file_name, int _x, int _y)
    {
        SDL_Surface* surface = SDL_LoadBMP(Catalog::build_resource_path("CURSORS/" + _file_name + ".bmp").c_str());
        if (!surface) {
            LOG_ERROR(ERR_MIN, "Failed to load cursor file '%s'!", _file_name.c_str());
            return;
        }
        defer { SDL_FreeSurface(surface); };

        u32 color_key = SDL_MapRGB(surface->format, 0xFF,0x00,0xFF);
        if (SDL_SetColorKey(surface, SDL_TRUE, color_key) < 0) {
            LOG_ERROR(ERR_MIN, "Failed to color key cursor '%s'!", _file_name.c_str());
            return;
        }

        cursors.insert(std::pair<std::string, SDL_Cursor*>(_file_name, SDL_CreateColorCursor(surface, _x, _y)));
        if (!cursors[_file_name]) {
            LOG_ERROR(ERR_MIN, "Failed to create cursor '%s'!", _file_name.c_str());
            return;
        }
    }
}

FILDEF void init_cursors ()
{
    internal::load_cursor("CURARROW",  6,  3);
    internal::load_cursor("CURBEAM" , 11, 12);
    internal::load_cursor("CURBRUSH",  3, 20);
    internal::load_cursor("CURPAINT", 19, 16);
    internal::load_cursor("CURCROSS", 11, 12);

    set_cursor(internal::current_cursor);
}
FILDEF void quit_cursors ()
{
    for (auto it: internal::cursors) {
        SDL_FreeCursor(it.second);
    }
    internal::cursors.clear();
}

FILDEF void set_cursor (std::string _name)
{
    if (internal::current_cursor != _name) {
        internal::current_cursor = _name;
        SDL_SetCursor(internal::cursors[internal::current_cursor]);
    }
}
FILDEF std::string get_cursor ()
{
    return internal::current_cursor;
}

FILDEF void show_cursor ()
{
    SDL_ShowCursor(SDL_ENABLE);
}
FILDEF void hide_cursor ()
{
    SDL_ShowCursor(SDL_DISABLE);
}

FILDEF void lock_mouse () { SDL_SetRelativeMouseMode(SDL_TRUE); hide_cursor(); }
FILDEF void unlock_mouse () { SDL_SetRelativeMouseMode(SDL_FALSE); show_cursor(); }
