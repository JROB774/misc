namespace Control
{
	enum State { STATE_NONE, STATE_OK, STATE_CLOSE };
	enum Type { TYPE_NEW, TYPE_RESIZE };

	// prototypes for the size getters
	inline int GetWidth();
	inline int GetHeight();

	namespace
	{
		// constant values regarding gui elements
		constexpr int GUI_WIDTH = 180, GUI_HEIGHT = 32;
		constexpr float GUI_RADIUS = 6.f;

		// constant values regarding text box
		constexpr int CURSOR_WIDTH = 2, CURSOR_HEIGHT = 24;
		constexpr int CURSOR_BLINK_INTERVAL = 16;
		constexpr int TEXTBOX_MAXCHAR = 4;

		// constant values regarding resize dir
		constexpr int DIR_BUTTON_SIZE = 32;
		constexpr int DIR_SIZE = DIR_BUTTON_SIZE * 3; // 3 per row/col

		// constant bounds for a level's size
		constexpr int MIN_LEVEL_WIDTH = 54, MIN_LEVEL_HEIGHT = 32;
		constexpr int MAX_LEVEL_WIDTH = 2000, MAX_LEVEL_HEIGHT = 2000;

		// each id (button) in the resize dir maps to a specific clip of the alignment texture
		constexpr GPU_Rect ALIGN_BOUNDS[9] = {
			{ 32, 32, 64, 64 }, { 0, 32, 96, 64 }, { 0, 32, 64, 64 },
			{ 32,  0, 64, 96 }, { 0,  0, 96, 96 }, { 0,  0, 64, 96 },
			{ 32,  0, 64, 64 }, { 0,  0, 96, 64 }, { 0,  0, 64, 64 }
		};

		struct TextBox
		{
			int x, y;
			std::string text;
			std::size_t cursor;
			unsigned int blink;
			bool focus;
		};

		struct Button
		{
			enum State { STATE_IDLE, STATE_PRESS, STATE_HOVER, STATE_TOTAL };

			int x, y, w, h;
			std::string text;
			SDL_Color areacol[STATE_TOTAL];
			SDL_Color textcol;
			Button::State state;
			Control::State output;
		};

		struct Resize
		{
			enum Dir { DIR_NW, DIR_N, DIR_NE, DIR_W, DIR_MD, DIR_E, DIR_SW, DIR_S, DIR_SE };

			int x, y;
			Dir dir;
			SDL_Color activecol;
			SDL_Color inactivecol;
			SDL_Color bordercol;
		};

		// prototype for the direction checkers
		inline bool IsNorth(Resize::Dir _dir);
		inline bool IsEast(Resize::Dir _dir);
		inline bool IsSouth(Resize::Dir _dir);
		inline bool IsWest(Resize::Dir _dir);

		// the color to grey out the screen with
		constexpr SDL_Color SCREEN_FILL = { 0x00, 0x00, 0x00, 0x6F };

		// the various size constants
		constexpr int WIDTH = 444;
		constexpr int NEW_HEIGHT = 136, RESIZE_HEIGHT = 240;
		// thickness of the box border
		constexpr int BORDER = 2;
		// curve of the control box
		constexpr float RADIUS = 6.f;

		// the control box's own data
		int x, y, w, h;
		Type type;
		State state;

		// the gui elements for the control box
		TextBox txt_width, txt_height;
		Button submit, close;
		// specific for the resize control box
		Resize resize;

		void CalculateArea ()
		{
			// determine the size and centered position of the box
			w = WIDTH, h = ((type == TYPE_NEW) ? NEW_HEIGHT : RESIZE_HEIGHT);
			x = (App::GetWidth() - w) / 2, y = (App::GetHeight() - h) / 2;
		}

		void TextBox_Handle (TextBox& _box)
		{
			switch (App::event.type) {
				// handles the focus for the text box
				case (SDL_MOUSEBUTTONDOWN): {
					if (Cursor::Within(x+_box.x, y+_box.y, GUI_WIDTH, GUI_HEIGHT)) {
						_box.focus = true;
						_box.blink = 0;
					}
					else { _box.focus = false; }
				} break;
				// handles the various text input events
				case (SDL_TEXTINPUT): {
					if (_box.focus) {
						// add the newly typed text to the text box in the correct position (only numbers)
						char ascii = App::event.text.text[0];
						if ((ascii >= '0' && ascii <= '9') && (_box.text.length() < TEXTBOX_MAXCHAR)) {
							_box.text.insert(_box.text.begin() + _box.cursor, ascii);
							// iterate the cursor and reset the blink counter
							_box.blink = 0;
							_box.cursor++;
						}
					}
				} break;
				// handling speacial chars
				case (SDL_KEYDOWN): {
					if (_box.focus) {
						switch (App::event.key.keysym.sym) {
							// erase a character before or after the curosr
							case (SDLK_BACKSPACE): { if (_box.cursor > 0) { _box.text.erase(--_box.cursor, 1); _box.blink = 0; } } break;
							case (SDLK_DELETE): { if (_box.cursor < _box.text.length()) { _box.text.erase(_box.cursor, 1); _box.blink = 0; } } break;
							// change your cursor position in the text
							case (SDLK_LEFT): { if (_box.cursor > 0) { --_box.cursor; _box.blink = 0; } } break;
							case (SDLK_RIGHT): { if (_box.cursor < _box.text.length()) { ++_box.cursor; _box.blink = 0; } } break;
						}
					}
				} break;
			}
		}

		void TextBox_Render (TextBox& _box)
		{
			// cache these values for optimisation
			int tx = x + _box.x, ty = y + _box.y;
			// draw the area/box of the text box
			GPU_RectangleRoundFilled(App::screen, tx,ty, tx+GUI_WIDTH,ty+GUI_HEIGHT, GUI_RADIUS, Render::Color::WHITE);

			// draw the internal text box text
			std::uint16_t fx = ((GUI_WIDTH - FC_GetWidth(Asset::fnt_standard, _box.text.c_str())) / 2) + tx;
			std::uint16_t fy = ((GUI_HEIGHT - FC_GetHeight(Asset::fnt_standard, _box.text.c_str())) / 2) + ty;
			Render::DrawFont(fx, fy, _box.text, Asset::fnt_standard, 1.f, 1.f, Render::Color::BLACK);

			// draw the cursor for the text box - only if has focus
			if (_box.focus) {
				// only if the cursor should be drawn
				if (_box.blink <= CURSOR_BLINK_INTERVAL) {
					// cache these values for optimisation
					std::uint32_t cx = fx + FC_GetWidth(Asset::fnt_standard, _box.text.substr(0, _box.cursor).c_str());
					std::uint16_t cy = ty + ((GUI_HEIGHT - CURSOR_HEIGHT) / 2);
					// draw the curosr at the correct position
					GPU_RectangleFilled(App::screen, cx, cy, cx+CURSOR_WIDTH, cy+CURSOR_HEIGHT, Render::Color::BLACK);
				}

				// iterate the blink counter
				if (_box.blink >= (CURSOR_BLINK_INTERVAL * 2)) { _box.blink = 0; }
				else { ++_box.blink; }
			}
		}

		void Button_Handle (Button& _btn)
		{
			switch (App::event.type) {
				// handle hovering over the button
				case (SDL_MOUSEMOTION): {
					if (_btn.state != Button::STATE_PRESS) {
						if (Cursor::Within(x+_btn.x, y+_btn.y, _btn.w, _btn.h)) { _btn.state = Button::STATE_HOVER; }
						else { _btn.state = Button::STATE_IDLE; }
					}
				} break;

				// handle button being pressed
				case (SDL_MOUSEBUTTONDOWN): { if (_btn.state == Button::STATE_HOVER) { _btn.state = Button::STATE_PRESS; } } break;
				// handle button being released
				case (SDL_MOUSEBUTTONUP): {
					if (_btn.state == Button::STATE_PRESS) {
						if (!Cursor::Within(x+_btn.x, y+_btn.y, _btn.w, _btn.h)) { _btn.state = Button::STATE_IDLE; }
						else {
							state = _btn.output; _btn.state = Button::STATE_HOVER;
							// ensure the width and height values are correct if submitting
							if (state == STATE_OK) {
								if (GetWidth() < MIN_LEVEL_WIDTH || GetHeight() < MIN_LEVEL_HEIGHT) {
									Platform::MessageBox(Platform::MSGBOX_TYPE_OK, "Invalid", "The width and height can't be below 54x32.");
									state = STATE_NONE; }
								else if (GetWidth() > MAX_LEVEL_WIDTH || GetHeight() > MAX_LEVEL_HEIGHT) {
									Platform::MessageBox(Platform::MSGBOX_TYPE_OK, "Invalid", "The width and height can't be above 2000x2000.");
									state = STATE_NONE; }
							}
						}
					}
				} break;
			}
		}

		void Button_Render (Button& _btn)
		{
			// cache these values for optimisation
			int bx = x + _btn.x, by = y + _btn.y;
			// draw the area/box of the button
			GPU_RectangleRoundFilled(App::screen, bx,by, bx+_btn.w,by+_btn.h, GUI_RADIUS, _btn.areacol[_btn.state]);

			// calculate the centerised rendering point for the font rendering
			std::uint16_t fx = ((_btn.w - FC_GetWidth(Asset::fnt_standard, _btn.text.c_str())) / 2) + bx;
			std::uint16_t fy = ((_btn.h - FC_GetHeight(Asset::fnt_standard, _btn.text.c_str())) / 2) + by;

			// draw the internal button text
			Render::DrawFont(fx, fy, _btn.text, Asset::fnt_standard, 1.f, 1.f, _btn.textcol);
		}

		void Resize_Handle (Resize& _res)
		{
			switch (App::event.type) {
				// handle the various resize alignment buttons being pressed
				case (SDL_MOUSEBUTTONDOWN): {
					// go through all the button positions to see if any were hit
					for (int iy = 0; iy < DIR_SIZE; iy += DIR_BUTTON_SIZE) {
						for (int ix = 0; ix < DIR_SIZE; ix += DIR_BUTTON_SIZE) {
							if (Cursor::Within(x+_res.x+ix, y+_res.y+iy, DIR_BUTTON_SIZE, DIR_BUTTON_SIZE)) {
								// get the alignment of the pressed button then quit
								_res.dir = (Resize::Dir)((iy/DIR_BUTTON_SIZE) * 3 + (ix/DIR_BUTTON_SIZE));
								return;
							}
						}
					}
				} break;
			}
		}

		void Resize_Render (Resize& _res)
		{
			// cache these values for optimisation
			int rx = x + _res.x, ry = y + _res.y;

			// draw the background borders of the resize align control and the buttons on top
			GPU_RectangleRoundFilled(App::screen, rx,ry, rx+DIR_SIZE,ry+DIR_SIZE, GUI_RADIUS, _res.bordercol);
			for (int iy = 0; iy < DIR_SIZE; iy += DIR_BUTTON_SIZE) {
				for (int ix = 0; ix < DIR_SIZE; ix += DIR_BUTTON_SIZE) {
					int bx1 = rx+ix, by1 = ry+iy, bx2 = bx1+DIR_BUTTON_SIZE, by2 = by1+DIR_BUTTON_SIZE;
					GPU_RectangleRoundFilled(App::screen, bx1,by1, bx2,by2, GUI_RADIUS, _res.inactivecol);
				}
			}

			// @IMPROVE: PRETTY OBVIOUS... NEEDS A BETTER SYSTEM AND WOULD BE BETTER CACHED RATHER THAN EVERY DRAW CALL
			// determine the location to draw the active alignment button
			int activex = rx+DIR_BUTTON_SIZE, activey = ry+DIR_BUTTON_SIZE; // default is the center pos
			if (IsNorth(_res.dir)) { activey = ry; }
			else if (IsSouth(_res.dir)) { activey = ry+(DIR_BUTTON_SIZE*2); }
			if (IsWest(_res.dir)) { activex = rx; }
			else if (IsEast(_res.dir)) { activex = rx+(DIR_BUTTON_SIZE*2); }

			// draw the currently active resize alignment on top
			GPU_RectangleRoundFilled(App::screen, activex,activey, activex+DIR_BUTTON_SIZE,activey+DIR_BUTTON_SIZE, GUI_RADIUS, _res.activecol);

			// draw the arrows that show what alignment each button handles
			int id = ((activey-ry)/DIR_BUTTON_SIZE) * 3 + ((activex-rx)/DIR_BUTTON_SIZE);
			GPU_Rect alignbounds = ALIGN_BOUNDS[id];
			int alignx = rx+(DIR_SIZE-(alignbounds.x+alignbounds.w)), aligny = ry+(DIR_SIZE-(alignbounds.y+alignbounds.h));
			Render::DrawTexture(alignx, aligny, Asset::tex_align, &alignbounds, Render::Color::WHITE);
		}

		inline bool IsNorth (Resize::Dir _dir) { return (_dir == Resize::DIR_NW || _dir == Resize::DIR_N || _dir == Resize::DIR_NE); }
		inline bool IsEast (Resize::Dir _dir) { return (_dir == Resize::DIR_NE || _dir == Resize::DIR_E || _dir == Resize::DIR_SE); }
		inline bool IsSouth (Resize::Dir _dir) { return (_dir == Resize::DIR_SW || _dir == Resize::DIR_S || _dir == Resize::DIR_SE); }
		inline bool IsWest (Resize::Dir _dir) { return (_dir == Resize::DIR_NW || _dir == Resize::DIR_W || _dir == Resize::DIR_SW); }
	}

	void Create (Type _type, int _lvlwidth = 0, int _lvlheight = 0)
	{
		// clear up the data for fresh/new use
		txt_width.text.clear(), txt_height.text.clear();
		txt_width.cursor = 0, txt_height.cursor = 0;
		txt_width.focus = false, txt_height.focus = false;

		// setup the control for use
		type = _type;
		state = STATE_NONE;

		// calculate the center position
		CalculateArea();

		// sets up the close Button
		close.w = 32, close.h = 32;
		close.x = WIDTH-close.w, close.y = 0;
		close.text = "X";
		close.areacol[Button::STATE_IDLE]  = { 0xE0, 0x00, 0x2C, 0xFF };
		close.areacol[Button::STATE_HOVER] = { 0xFF, 0x00, 0x33, 0xFF };
		close.areacol[Button::STATE_PRESS] = { 0xB2, 0x10, 0x30, 0xFF };
		close.textcol = Render::Color::WHITE;
		close.output = STATE_CLOSE;

		// sets up the submit button
		submit.w = GUI_WIDTH, submit.h = GUI_HEIGHT;
		submit.x = (w-submit.w)/2, submit.y = (h-submit.h)-18;
		submit.text = "OK";
		submit.areacol[Button::STATE_IDLE]  = { 0xAF, 0xAF, 0xAF, 0xFF };
		submit.areacol[Button::STATE_HOVER] = { 0xCF, 0xCF, 0xCF, 0xFF };
		submit.areacol[Button::STATE_PRESS] = { 0x80, 0x80, 0x80, 0xFF };
		submit.textcol = Render::Color::BLACK;
		submit.output = STATE_OK;

		// sets up the text boxes
		txt_width.x = 24, txt_width.y = 42;
		txt_height.x = (w-GUI_WIDTH)-24, txt_height.y = 42;
		// the default content of the text boxes will depend on if new or resize control
		if (type == TYPE_NEW) { txt_width.text = "54", txt_height.text = "32"; }
		else { txt_width.text = std::to_string(_lvlwidth), txt_height.text = std::to_string(_lvlheight); }
		// cursor set to the end of the text string
		txt_width.cursor = txt_width.text.length(), txt_height.cursor = txt_height.text.length();

		// if we're a resize control we want to setup the resize direction input as well
		if (type == TYPE_RESIZE) {
			resize.x = (WIDTH-DIR_SIZE)/2;
			resize.y = (txt_width.y+GUI_HEIGHT)+(((submit.y-(txt_width.y+GUI_HEIGHT))-DIR_SIZE)/2); // @IMPROVE: COULD BE SIMPLER...
			resize.dir = Resize::DIR_MD;
			resize.activecol = Render::Color::WHITE;
			resize.inactivecol = { 0xAF, 0xAF, 0xAF, 0xFF };
			resize.bordercol = { 0x40, 0x40, 0x40, 0xFF };
		}

		// check the state of the buttons on startup (mouse could be on top)
		if (Cursor::Within(x+submit.x, y+submit.y, submit.w, submit.h)) { submit.state = Button::STATE_HOVER; }
		else { submit.state = Button::STATE_IDLE; }
		if (Cursor::Within(x+close.x, y+close.y, close.w, close.h)) { close.state = Button::STATE_HOVER; }
		else { close.state = Button::STATE_IDLE; }

		// enable sdl text input handling for the text boxes
		SDL_StartTextInput();
	}

	void Handle ()
	{
		// handle global prompt/control events
		switch (App::event.type) {
			case (SDL_KEYUP): {
				// acts as if close is pressed
				if (App::event.key.keysym.sym == SDLK_ESCAPE) { state = STATE_CLOSE; }
				// acts as if submit is pressed
				else if (App::event.key.keysym.sym == SDLK_RETURN) {
					state = STATE_OK;
					// ensure the width and height values are correct
					if (GetWidth() < 54 || GetHeight() < 32) {
						Platform::MessageBox(Platform::MSGBOX_TYPE_OK, "Invalid", "The width and height can't be below 54x32.");
						state = STATE_NONE;
					}
				}
			} break;
		}

		// handle events for the text boxes
		TextBox_Handle(txt_width);
		TextBox_Handle(txt_height);
		// handle events for the submit button
		Button_Handle(submit);
		Button_Handle(close);

		// handles events for the resizer
		if (type == TYPE_RESIZE) { Resize_Handle(resize); }
	}

	void Render ()
	{
		// grey out the screen to show only the control is relevant
		GPU_RectangleFilled(App::screen, 0, 0, App::GetWidth(), App::GetHeight(), SCREEN_FILL);

		// the actual control/prompt box area and its outline
		GPU_RectangleRoundFilled(App::screen, x-BORDER, y-BORDER, (x+w)+BORDER, (y+h)+BORDER, RADIUS, Render::Color::GREY);
		GPU_RectangleRoundFilled(App::screen, x, y, x+w, y+h, RADIUS, Render::Color::BLACK);

		// calculate the centerised rendering point for the X rendering
		std::uint16_t fx = (App::GetWidth() - FC_GetWidth(Asset::fnt_standard, "X")) / 2;
		std::uint16_t fy = y + txt_width.y + ((GUI_HEIGHT - FC_GetHeight(Asset::fnt_standard, "X")) / 2);
		// render the X between the two dimension text boxes
		Render::DrawFont(fx, fy, "X", Asset::fnt_standard, 1.f, 1.f, Render::Color::WHITE);

		// draw the input text boxes
		TextBox_Render(txt_width);
		TextBox_Render(txt_height);
		// draw the submit button
		Button_Render(submit);
		Button_Render(close);

		// draw the resizer control
		if (type == TYPE_RESIZE) { Resize_Render(resize); }
	}

	inline Type GetType ()
	{
		return type;
	}

	inline int GetWidth ()
	{
		return (txt_width.text.empty()) ? 0 : std::stoi(txt_width.text);
	}

	inline int GetHeight ()
	{
		return (txt_height.text.empty()) ? 0 : std::stoi(txt_height.text);
	}

	inline State GetState ()
	{
		return state;
	}

	inline Resize::Dir GetDir ()
	{
		return resize.dir;
	}
}