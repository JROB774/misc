namespace Hotbar
{
	constexpr int HEIGHT = 50;

	enum ButtonId
	{
		BUTTON_NEW,
		BUTTON_OPEN,
		BUTTON_SAVE,
		BUTTON_SAVEAS,
		BUTTON_GPAK_UNPACK,
		BUTTON_GPAK_PACK,
		BUTTON_UNDO,
		BUTTON_REDO,
		BUTTON_RESIZE,
		BUTTON_FLIP_HORZ,
		BUTTON_FLIP_VERT,
		BUTTON_MIRRORTOGGLE_HORZ,
		BUTTON_MIRRORTOGGLE_VERT,
		BUTTON_PENCIL,
		BUTTON_FILL,
		BUTTON_SELECT,
		BUTTON_GRIDTOGGLE,
		BUTTON_CAMERATOGGLE,
		BUTTON_RUN,
		BUTTON_RUNDEBUG,
		BUTTON_SETTINGS,
		BUTTON_HELP,
		BUTTON_TOTAL
	};

	namespace
	{
		struct Button
		{
			int x, y;
			GPU_Rect bounds;
			void(*action)();
			bool hot, hit;
		};

		constexpr const char* TOOLTIP[BUTTON_TOTAL] = {
			"New", "Open", "Save", "Save As", "GPAK Unpack", "GPAK Pack",
			"Undo", "Redo", "Resize", "Horizontal Flip", "Vertical Flip",
			"Horizontal Symmetry", "Vertical Symmetry", "Brush", "Fill",
			"Select", "Toggle Grid", "Toggle Camera Bounds", "Run",
			"Run Debug", "Settings", "Help"
		};

		constexpr float BUTTON_SPR_SIZE = 64.f;
		constexpr float BUTTON_SIZE = 40.f;
		constexpr float BUTTON_SCALE = BUTTON_SIZE / BUTTON_SPR_SIZE;
		constexpr float BUTTON_SHRINK = .08f;
		constexpr float BUTTON_EXPAND = .1f;
		constexpr float BUTTON_PAD = 10.f;
		constexpr float BUTTON_OFFSET = BUTTON_SIZE / 2.0f;
		constexpr float BUTTON_ANCHOR = BUTTON_SPR_SIZE / 2.0f;

		Button* hotbutton = nullptr;
		Button* hitbutton = nullptr;

		std::array<Button, BUTTON_TOTAL> buttons = {};

		GPU_Rect bounds;
		SDL_Color color, buttoncolor;

		void CalculateArea ()
		{
			// resizes the hotbar area
			bounds.w = App::GetWidth();
		}
	}

	void Init ()
	{
		// this will change with the window allowing for scaling
		bounds = { 0, 0, (float)App::GetWidth(), HEIGHT };

		// sets up all the hotbar buttons...
		std::size_t index = 0;
		for (auto& button: buttons) {
			// creates graphics and bounds
			button.x = (BUTTON_SIZE * index) + (BUTTON_PAD * (index + 1));
			button.y = BUTTON_PAD / 2.f;
			button.bounds = {
				BUTTON_SPR_SIZE * index, 0,
				BUTTON_SPR_SIZE, BUTTON_SPR_SIZE
			};
			++index;
		}

		// sets the color of the hotbar and buttons graphics
		color = Settings::GetColor("hotbar_color");
		buttoncolor = Settings::GetColor("hotbar_buttons_color");
	}

	void Handle ()
	{
		// handle events for the hotbar as a whole
		switch (App::event.type) {
			// if the mouse is over a button then make it hot
			case (SDL_MOUSEMOTION): {
				if (!hotbutton) {
					// handles setting the current hot button
					std::size_t index = 0;
					for (auto& button: buttons) {
						if (Cursor::Within(button.x, button.y, BUTTON_SIZE, BUTTON_SIZE)) {
							hotbutton = &button;
							hotbutton->hot = true;
							Tooltip::Set(TOOLTIP[index], std::string());
							break;
						}
						++index;
					}
				}
				else {
					// handles unsetting the current hot button
					if (!Cursor::Within(hotbutton->x, hotbutton->y, BUTTON_SIZE, BUTTON_SIZE)) {
						hotbutton->hot = false;
						hotbutton->hit = false;
						hotbutton = nullptr;
						Tooltip::Unset();
					}
				}
			} break;
			// if the mouse is pressed/released perform appropriate action
			case (SDL_MOUSEBUTTONDOWN): {
				// don't care if the mouse was pressed if not hot or LMB
				if (hotbutton && App::event.button.button == SDL_BUTTON_LEFT) {
					hitbutton = hotbutton;
					hitbutton->hit = true;
				}
			} break;
			case (SDL_MOUSEBUTTONUP): {
				// only perform the action if still over the same button
				if (hitbutton) {
					if (hitbutton == hotbutton && hitbutton->action) { // don't want to call nulllptr
						hitbutton->action();
						hitbutton->hit = false;
					}
					hitbutton = nullptr;
				}
			} break;
		}
	}

	void Render ()
	{
		// renders the hotbar boundaries
		GPU_RectangleFilled(
			App::screen,
			bounds.x, bounds.y,
			bounds.x+bounds.w, bounds.y+bounds.h,
			color
		);

		// renders all of the buttons
		float scalemod;
		for (auto& button: buttons) {
			// if the button is hit or hot, add the appropriate scale mod
			if (button.hit) { scalemod = -BUTTON_SHRINK; }
			else if (button.hot) { scalemod = BUTTON_EXPAND; }
			else { scalemod = 0.f; }
			// render the button itself
			Render::DrawTexture(
				button.x + BUTTON_OFFSET, button.y + BUTTON_OFFSET,
				BUTTON_ANCHOR, BUTTON_ANCHOR,
				BUTTON_SCALE + scalemod, BUTTON_SCALE + scalemod,
				Asset::tex_hotbar_buttons,
				&button.bounds,
				buttoncolor
			);
		}
	}

	void SetAction (ButtonId _id, void(*_action)())
	{
		buttons.at(_id).action = _action;
	}

	void ToggleGraphic (ButtonId _id)
	{
		// toggle the button graphic
		buttons.at(_id).bounds.y += BUTTON_SPR_SIZE;
		// stops it from going out of the image bounds
		if (buttons.at(_id).bounds.y >= Asset::tex_hotbar_buttons->h) {
			buttons.at(_id).bounds.y = 0;
		}
	}
}
