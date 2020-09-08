namespace Cursor
{
	constexpr int SIZE = 32;

	enum Type
	{
		CURSOR_ARROW,
		CURSOR_PENCIL,
		CURSOR_FILL,
		CURSOR_SELECT,
		CURSOR_TOTAL
	};

	namespace
	{
		constexpr const char* NAME[CURSOR_TOTAL] = { "arrow.png", "pencil.png", "fill.png", "select.png" };
		constexpr SDL_Point POS[CURSOR_TOTAL] = { {  0,  0 }, {  0, 31 }, {  3, 19 }, { 16, 16 } };

		std::array<SDL_Cursor*, CURSOR_TOTAL> cursors;

		bool use_system_cursor = false;
		bool unique_tool_cursors = true;
		bool show_tool_cursors = true;
	}

	inline bool Within (int _x, int _y, int _w, int _h)
	{
		// gets the current position of the mouse
		int x, y;
		SDL_GetMouseState(&x, &y);

		// checks to see if it is within the given bounds
		return ((x >= _x) && (x < (_x + _w)) && \
			    (y >= _y) && (y < (_y + _h)));
	}

	void Init ()
	{
		// store flags regarding the cursor and how it should be displayed
		show_tool_cursors = Settings::GetBoolean("show_cursor_using_tools");
		unique_tool_cursors = Settings::GetBoolean("use_unique_tool_cursors");
		// if we're using the system cursor in the first place, don't bother with the init
		use_system_cursor = Settings::GetBoolean("use_system_cursor");
		if (use_system_cursor) { return; }

		std::size_t index = 0;
		for (auto& cursor: cursors) {
			// loads the cursor PNG into a surface for cursor creation
			std::string name = NAME[index];
			SDL_RWops* stream = DataManager::Load("cursor_" + name);
			if (!stream) {
				ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_STD, "Failed to load cursor PNG '" + name + "'!");
				use_system_cursor = true;
				break;
			}
			SDL_Surface* surface = GPU_LoadSurface_RW(stream, true);
			if (!surface) {
				ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_GPU, "Failed to convert cursor '" + name + "'!");
				use_system_cursor = true;
				break;
			}

			// creates the current cursor using the image data
			cursor = SDL_CreateColorCursor(surface, POS[index].x, POS[index].y);
			if (!cursor) {
				ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_SDL, "Failed to create cursor '" + name + "'!");
				use_system_cursor = true;
				break;
			}

			// deallocates no longer needed memory
			SDL_FreeSurface(surface);
			surface = nullptr;

			// move on
			++index;
		}

		// if we failed to load a cursor we just fall-back to the system cursor
		if (use_system_cursor) { return; }

		// sets the cursor to be the default arrow
		SDL_SetCursor(cursors.at(CURSOR_ARROW));
	}

	void Quit ()
	{
		// deallocates cursor memory after use
		for (auto& cursor: cursors) { SDL_FreeCursor(cursor); }
	}

	void SetCursor (Type _type)
	{
		// if we're showing cursors for tools and not the system cursor, just set normally
		if (show_tool_cursors) {
			// either set unique cursor tool graphics or just use the normal cursor
			if (!use_system_cursor) {
				if (unique_tool_cursors) { SDL_SetCursor(cursors.at(_type));  }
				else { SDL_SetCursor(cursors.at(CURSOR_ARROW)); }
			}
		}
		// if we're not showing cursors for tools, hide and show the cursor appropriately
		else {
			if (_type == CURSOR_ARROW || _type == CURSOR_SELECT) {
				SDL_ShowCursor(SDL_ENABLE);
				if (unique_tool_cursors && !use_system_cursor) {
					SDL_SetCursor(cursors.at(_type));
				}
			}
			else { SDL_ShowCursor(SDL_DISABLE); }
		}
	}
}