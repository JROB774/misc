namespace TilePanel
{
	constexpr int WIDTH = 270;
	constexpr int SPR_COLUMNS = 10;

	constexpr float TILE_SPR_SIZE = 64.f;
	constexpr float TILE_SPR_PADDING = 8.f;

	// @TEMP: PROTOTYPING FOR USE IN PRIVATE FUNCTIONS
	std::uint32_t GetActiveID();
	std::uint8_t GetActiveLayer();
	std::uint8_t GetActiveCategory();
	bool AllLayersAreInactive();

	namespace
	{
		struct Group
		{
			float x, y;
			std::string name;
			std::string desc;
			std::vector<std::uint32_t> ids;
			int activeid; // int not size_t so scrolling doesn't underflow
			GPU_Rect bounds;
			bool active;
		};

		struct Category
		{
			float texty;
			std::string name;
			std::vector<Group> groups;
		};

		constexpr int COLUMNS = 9;
		constexpr int ROWS = 17;
		constexpr int TOTAL = COLUMNS * ROWS;
		constexpr int PADDING = 4;
		constexpr int BACK2_START = LVL::OFFSET[LVL::CATEGORY_BACK] + 33;
		constexpr int BACK2_START_GROUP = 7;

		constexpr float TILE_SIZE = 25.f;
		constexpr float TILE_SCALE = TILE_SIZE / TILE_SPR_SIZE;

		constexpr std::uint32_t LABEL_HEIGHT = 28;

		constexpr int CONTENT_WIDTH = (COLUMNS * TILE_SIZE) + (PADDING * (COLUMNS + 1));
		constexpr int CONTENT_HEIGHT = (ROWS * TILE_SIZE) + (PADDING * (ROWS + 1)) + (LABEL_HEIGHT * LVL::CATEGORY_TOTAL);

		constexpr int INACTIVE_BORDER_SIZE = 1;
		constexpr int ACTIVE_BORDER_SIZE = 2;

		std::uint8_t inactive_tile_alpha;

		SDL_Color selected_tile_color;
		SDL_Color unselected_tile_color;
		SDL_Color color;
		SDL_Color text_color;

		GPU_Rect bounds;

		std::array<Category, LVL::CATEGORY_TOTAL> categories;
		std::array<bool, LVL::LAYER_TOTAL> islayeractive = { true, true, true, true, true };

		// so our starting tile is the solid tile in category basic
		int activecategory = LVL::CATEGORY_BASIC;
		int activegroup = 0;

		// used for tooltips
		Group* hotgroup = nullptr;

		// this flag is set when no layers/categories are active
		bool inactive = false;

		void CalculateArea ()
		{
			// resizes the tilepanel area
			bounds.h = App::GetHeight();
			// repositions the tile panel content
			int contenty = ((App::GetHeight() - CONTENT_HEIGHT) / 2) + (Hotbar::HEIGHT / 2);
			int y = contenty + PADDING;
			int counter = 0;
			for (auto& category: categories) {
				category.texty = y + ((LABEL_HEIGHT - Asset::FONT_SIZE) / 2);
				y += LABEL_HEIGHT;
				for (auto& group: category.groups) {
					group.y = y;
					++counter;
					if (counter >= COLUMNS) {
						y += (TILE_SIZE + PADDING);
						counter = 0;
					}
				}
				y += (TILE_SIZE + PADDING);
				counter = 0;
			}
		}

		void TileCycle (int _direction)
		{
			// get the currently active group, saves on writing all of that evey time
			auto& group = categories.at(activecategory).groups.at(activegroup);
			// set the new active tile in the group
			group.activeid += _direction;
			// bounds checking
			if (group.activeid >= (int)group.ids.size()) { group.activeid = 0; }
			else if (group.activeid < 0) { group.activeid = group.ids.size()-1; }
			// adjust the displayed graphic appropriately
			int posid = group.ids.at(group.activeid) + LVL::OFFSET[activecategory];
			group.bounds.x = TILE_LOOKUP[posid].x;
			group.bounds.y = TILE_LOOKUP[posid].y;
		}

		void TileBack () { TileCycle(-1); }
		void TileForward () { TileCycle(+1); }
	
		void GroupCycle (int _direction)
		{
			// we could get stuck in an infinite loop
			if (inactive) { return; }

			// set the new active group in the category
			activegroup += _direction;
			// bounds checking
			if (activegroup >= (int)categories.at(activecategory).groups.size()) { activegroup = 0; }
			else if (activegroup < 0) { activegroup = categories.at(activecategory).groups.size()-1; }

			// the only time we need to handle inactive tiles at a group level is back1+back2
			if (activecategory == LVL::CATEGORY_BACK) {
				auto& groups = categories.at(activecategory).groups;
				while (!groups.at(activegroup).active) {
					activegroup += _direction;
					// bounds checking @TODO/OPTIMISE: FIND A WAY TO ORGANISE THIS SO NO NEED TO COPY
					if (activegroup >= (int)categories.at(activecategory).groups.size()) { activegroup = 0; }
					else if (activegroup < 0) { activegroup = categories.at(activecategory).groups.size()-1; }
				}
			}
		}

		void GroupBack () { GroupCycle(-1); }
		void GroupForward () { GroupCycle(+1); }

		void CategoryCycle (int _direction)
		{
			// we will get stuck in an infinite loop
			if (inactive) { return; }

			do {
				// set the new active category (always places you at group 0)
				activecategory += _direction, activegroup = 0; // also prevents potential out_of_bounds exceptions
				// bounds checking
				if (activecategory >= (int)categories.size()) { activecategory = 0; }
				else if (activecategory < 0) { activecategory = categories.size()-1; }
				// special handling for the weird case that is back1 + back2
				if (activecategory == LVL::CATEGORY_BACK && !islayeractive.at(LVL::LAYER_BACK1)) { activegroup = BACK2_START_GROUP; }
			}
			while (!islayeractive.at(GetActiveLayer()));
		}

		void CategoryBack () { CategoryCycle(-1); }
		void CategoryForward () { CategoryCycle(+1); }

		void SetCategoryBasic ()   { if (islayeractive.at(LVL::LAYER_ACTIVE)) { activecategory = LVL::CATEGORY_BASIC; activegroup = 0; } }
		void SetCategoryTag ()     { if (islayeractive.at(LVL::LAYER_TAG)) { activecategory = LVL::CATEGORY_TAG; activegroup = 0; } }
		void SetCategoryOverlay () { if (islayeractive.at(LVL::LAYER_OVERLAY)) { activecategory = LVL::CATEGORY_OVERLAY; activegroup = 0; } }
		void SetCategoryEnemy ()   { if (islayeractive.at(LVL::LAYER_ACTIVE)) { activecategory = LVL::CATEGORY_ENEMY; activegroup = 0; } }
		void SetCategoryBack ()
		{
			// this one just has to be different... -_-
			if (islayeractive.at(LVL::LAYER_BACK1)) { activecategory = LVL::CATEGORY_BACK; activegroup = 0; }
			else if (islayeractive.at(LVL::LAYER_BACK2))  { activecategory = LVL::CATEGORY_BACK; activegroup = BACK2_START_GROUP; }
		}
	}

	void Init ()
	{
		// this will change with the window allowing for scaling
		bounds = { 0, 0, WIDTH, (float)App::GetHeight() };

		// creates the tiles, groups, and categories made to organise tile info
		// searches the tile info list and adds tiles to the appropriate categories
		// and sub-category (group) based on given info

		// opens the tile info to extract/read information on each tile
		SDL_RWops* stream = DataManager::Load("tileinfo.dat");
		std::string tiledata;
		tiledata.resize(SDL_RWsize(stream));
		if (SDL_RWread(stream, (void*)&tiledata[0], SDL_RWsize(stream), 1) == 0) {
			ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_SDL, "Failed to read tile info file!");
		}
		// cleans up the stream after use as it's no longer needed
		SDL_RWclose(stream);
		stream = nullptr;

		// sets all the category names
		categories.at(LVL::CATEGORY_BASIC).name = "Basics";
		categories.at(LVL::CATEGORY_TAG).name = "Tags";
		categories.at(LVL::CATEGORY_OVERLAY).name = "Overlays";
		categories.at(LVL::CATEGORY_ENEMY).name = "Enemies";
		categories.at(LVL::CATEGORY_BACK).name = "Backgrounds";

		// reads the data line-by-line to get each group's information
		auto lines = Utility::StrToken(tiledata, "\n");
		for (auto& line: lines) {
			auto tokens = Utility::StrToken(line, "|");
			// adds a new tile group entry to the appropriate category
			std::uint8_t category = std::stoi(tokens.at(0));
			categories.at(category).groups.push_back(Group {}); // zero initialise all
			categories.at(category).groups.back().name = tokens.at(1);
			categories.at(category).groups.back().desc = tokens.at(2);
			categories.at(category).groups.back().active = true;
			// adds all the tiles that are relevant to the group
			for (std::size_t i = 3; i < tokens.size(); ++i) {
				categories.at(category).groups.back().ids.push_back(
					std::stoi(tokens.at(i))
				);
			}
		}

		// calculates the position of the tile panel content
		int contentx = (WIDTH - CONTENT_WIDTH) / 2;
		int contenty = ((App::GetHeight() - CONTENT_HEIGHT) / 2) + (Hotbar::HEIGHT / 2);

		// sets up the tile panel visually, assigning each group a graphic, etc
		int x = contentx + PADDING, y = contenty + PADDING;
		int categoryindex = 0;
		for (auto& category: categories) {
			// adds the height of the categroy label/name
			category.texty = y + ((LABEL_HEIGHT - Asset::FONT_SIZE) / 2);
			y += LABEL_HEIGHT;
			// handles the tile groups...
			for (auto& group: category.groups) {
				// sets the group's positiion on-screen
				group.x = x, group.y = y;
				// gets the position in the spritesheet of the tile graphic
				int posid = group.ids.front() + LVL::OFFSET[categoryindex];
				float posx = TILE_LOOKUP[posid].x;
				float posy = TILE_LOOKUP[posid].y;
				// sets the first tile id in the group as the graphic
				group.bounds = { posx, posy, TILE_SPR_SIZE, TILE_SPR_SIZE };
				// goes to the start of a new row (row full)
				x += (TILE_SIZE + PADDING);
				if (x >= (contentx + CONTENT_WIDTH)) {
					x = contentx + PADDING;
					y += (TILE_SIZE + PADDING);
				}
			}
			// goes to the start of a new row (new category)
			x = contentx + PADDING;
			y += (TILE_SIZE + PADDING);
			++categoryindex;
		}

		// sets the different color values for the tile panel
		selected_tile_color = Settings::GetColor("tilepanel_selected_tile_color");
		unselected_tile_color = Settings::GetColor("tilepanel_unselected_tile_color");
		color = Settings::GetColor("tilepanel_color");
		text_color = Settings::GetColor("tilepanel_text_color");
		// and one alpha value as well
		inactive_tile_alpha = Settings::GetInteger("tilepanel_inactive_tile_alpha");

		// @TOOD: MODIFY THROUGH SETTINGS MENU
		// assigns hotkeys/shortcuts for various actions
		Hotkey::Register(KMOD_NONE, SDLK_q, TileBack);
		Hotkey::Register(KMOD_NONE, SDLK_e, TileForward);
		Hotkey::Register(KMOD_ALT, SDLK_q, GroupBack);
		Hotkey::Register(KMOD_ALT, SDLK_e, GroupForward);
		Hotkey::Register(KMOD_ALT|KMOD_SHIFT, SDLK_q, CategoryBack);
		Hotkey::Register(KMOD_ALT|KMOD_SHIFT, SDLK_e, CategoryForward);
		Hotkey::Register(KMOD_NONE, SDLK_1, SetCategoryBasic);
		Hotkey::Register(KMOD_NONE, SDLK_2, SetCategoryTag);
		Hotkey::Register(KMOD_NONE, SDLK_3, SetCategoryOverlay);
		Hotkey::Register(KMOD_NONE, SDLK_4, SetCategoryEnemy);
		Hotkey::Register(KMOD_NONE, SDLK_5, SetCategoryBack);
	}

	void Handle ()
	{
		switch (App::event.type) {
			// checks to see if any of the tile groups are moused over (sets tooltip)
			case (SDL_MOUSEMOTION): {
				if (!hotgroup) {
					// handles setting the current hot group
					for (auto& category: categories) {
						for (auto& group: category.groups) {
							if (group.active && Cursor::Within(group.x, group.y, TILE_SIZE, TILE_SIZE)) {
								hotgroup = &group;
								Tooltip::Set(group.name, group.desc);
							}
						}
					}
				}
				else {
					// handles unsetting the current hot group
					if (!Cursor::Within(hotgroup->x, hotgroup->y, TILE_SIZE, TILE_SIZE)) {
						hotgroup = nullptr;
						Tooltip::Unset();
					}
				}
			} break;
			// checks to see if any of the tile groups have been clicked (sets active)
			case (SDL_MOUSEBUTTONDOWN): {
				if (App::event.button.button == SDL_BUTTON_LEFT) {
					if (Cursor::Within(0, 0, WIDTH, App::GetHeight())) {
						// goes through and sees if it was pressed over any of the groups
						std::size_t categoryindex = 0;
						std::size_t groupindex = 0;
						for (auto& category: categories) {
							for (auto& group: category.groups) {
								// not allowed to select inactive groups, no point anyway as the layer would be locked
								if (group.active && Cursor::Within(group.x, group.y, TILE_SIZE, TILE_SIZE)) {
									activecategory = categoryindex;
									activegroup = groupindex;				
								}
								groupindex++;
							}
							categoryindex++;
							groupindex = 0;
						}
					}
				}
			} break;
			// checks to see if the mouse-wheel is scrolled (cycles tiles in the active group)
			case (SDL_MOUSEWHEEL): {
				// cycles through either tiles, groups, or categories, based on key mods
				int direction = Utility::Sign(App::event.wheel.y);
				     if (Hotkey::CheckKeyMods(KMOD_NONE))           { TileCycle(direction); }
				else if (Hotkey::CheckKeyMods(KMOD_ALT))            { GroupCycle(direction); }
				else if (Hotkey::CheckKeyMods(KMOD_ALT|KMOD_SHIFT)) { CategoryCycle(direction); }
			} break;
		}
	}

	void Render ()
	{
		// renders the tilepanel boundaries
		GPU_RectangleFilled(App::screen, bounds.x, bounds.y, bounds.x+bounds.w, bounds.y+bounds.h, color);

		// render all of the tile groups within the tile panel (and their borders)
		std::size_t categoryindex = 0;
		std::size_t groupindex = 0;
		int border;
		SDL_Color color;
		for (auto& category: categories) {
			// render the name/label for the current category
			Render::DrawFont(4.f, category.texty, category.name, Asset::fnt_standard, 1.f, 1.f, text_color);
			// render the groups contained in the category
			for (auto& group: category.groups)  {
				// if the group is currently active, it gets a bigger border and different color
				// this will never be handled if all layers are inactive
				if (!inactive && (int)groupindex == activegroup && (int)categoryindex == activecategory) { border = ACTIVE_BORDER_SIZE; color = selected_tile_color; }
				else { border = INACTIVE_BORDER_SIZE; color = unselected_tile_color; }
				// if the group is not active/visible then transparency is applied
				if (!group.active) { color.a = inactive_tile_alpha; }
				// draws the border around the group
				GPU_RectangleFilled(App::screen, group.x-border, group.y-border, group.x+TILE_SIZE+border, group.y+TILE_SIZE+border, color);
				// draws the active tile in the group
				Render::DrawTexture(group.x, group.y, 0.f, 0.f, TILE_SCALE, TILE_SCALE, Asset::tex_tiles, &group.bounds, color);
				// moves on to next group
				++groupindex;
			}
			// moves on to next category
			++categoryindex;
			groupindex = 0;
		}
	}
	
	void ToggleLayer (std::uint8_t _layer)
	{
		// toggles whether tiles can be selected or accessed from the tile panel this function is called from in
		// the level editor whenever a layer is set to active/inactive (has its content locked)

		// first it sets the state, so it knows whether is enabling/disabling
		islayeractive.at(_layer) = !islayeractive.at(_layer);

		// determines what needs enabling/disabling in each categroy based on the layer
		switch (_layer) {
			case (LVL::LAYER_ACTIVE): {
				for (auto& group: categories.at(LVL::CATEGORY_BASIC).groups) { group.active = islayeractive.at(_layer); }
				for (auto& group: categories.at(LVL::CATEGORY_ENEMY).groups) { group.active = islayeractive.at(_layer); }
			} break;
			case (LVL::LAYER_TAG): { for (auto& group: categories.at(LVL::CATEGORY_TAG).groups) { group.active = islayeractive.at(_layer); } } break;
			case (LVL::LAYER_OVERLAY): { for (auto& group: categories.at(LVL::CATEGORY_OVERLAY).groups) { group.active = islayeractive.at(_layer); } } break;
			case (LVL::LAYER_BACK1): {
				for (std::size_t i = 0; i < BACK2_START_GROUP; ++i) {
					categories.at(LVL::CATEGORY_BACK).groups.at(i).active = islayeractive.at(_layer);
				}
			} break;
			case (LVL::LAYER_BACK2): {
				for (std::size_t i = BACK2_START_GROUP; i < categories.at(LVL::CATEGORY_BACK).groups.size(); ++i) {
					categories.at(LVL::CATEGORY_BACK).groups.at(i).active = islayeractive.at(_layer);
				}
			} break;
		}

		// removes the tooltip for the current hotgroup if it became inactive
		if (hotgroup) { if (!hotgroup->active) { hotgroup = nullptr; Tooltip::Unset(); } }

		// see if all layers are inactive or not
		bool isanylayeractive = false;
		int theactivelayerid = -1;
		for (int i = 0; i < LVL::LAYER_TOTAL; ++i) {
			if (islayeractive.at(i)) {
				isanylayeractive = true;
				theactivelayerid = i;
				break;
			}
		}

		// only do this is a layer is active otherwise it'll infinite loop
		if (isanylayeractive) {
			// if the inactive flag is currently set we need to unset it and now move to the newly
			// activated layer, as there were no active layers before
			if (inactive) {
				inactive = false;
				activegroup = 0;
				switch (theactivelayerid) {
					case (LVL::LAYER_ACTIVE):  { activecategory = LVL::CATEGORY_BASIC;                                 } break;
					case (LVL::LAYER_TAG):     { activecategory = LVL::CATEGORY_TAG;                                   } break;
					case (LVL::LAYER_OVERLAY): { activecategory = LVL::CATEGORY_OVERLAY;                               } break;
					case (LVL::LAYER_BACK1):   { activecategory = LVL::CATEGORY_BACK;                                  } break;
					case (LVL::LAYER_BACK2):   { activecategory = LVL::CATEGORY_BACK; activegroup = BACK2_START_GROUP; } break;
				}
			}
			// we put this in an else to prevent more infinite loops
			else {
				// if you're the current active group in in a now inactive category we have to move you out
				while (!islayeractive.at(GetActiveLayer())) {
					if (activecategory != LVL::CATEGORY_BACK) { CategoryForward(); }
					else {
						// special case for dealing with back category/layers
						if (activegroup < BACK2_START_GROUP) { activegroup = BACK2_START_GROUP; }
						else { CategoryForward(); }
					}
				}
			}
		}
		// if all layers are disabled we set the inactive flag which will disable the rendering of an active
		// tile and inform the level editor not to render a ghosted tile, when a layer is then enabled once
		// more it is immediately set to being active... all this category/layer stuff is a mess...
		else { inactive = true; }
	}

	std::uint8_t CategoryToLayer (std::uint8_t _category, std::uint32_t _id)
	{
		// have to do this shit because categories and layers are different for some reason...
		switch (_category) {
			case (LVL::CATEGORY_BASIC):
			case (LVL::CATEGORY_ENEMY):   { return LVL::LAYER_ACTIVE; }
			case (LVL::CATEGORY_TAG):     { return LVL::LAYER_TAG; }
			case (LVL::CATEGORY_OVERLAY): { return LVL::LAYER_OVERLAY; }
			case (LVL::CATEGORY_BACK):    {
				if (_id >= BACK2_START) { return LVL::LAYER_BACK2; }
				else { return LVL::LAYER_BACK1; }
			}
		}

		return 0;
	}

	std::uint32_t GetActiveID ()
	{
		auto& group = categories.at(activecategory).groups.at(activegroup);
		return group.ids.at(group.activeid) + LVL::OFFSET[activecategory];
	}

	std::uint8_t GetActiveLayer ()
	{
		return CategoryToLayer(activecategory, GetActiveID());
	}

	std::uint8_t GetActiveCategory ()
	{
		return activecategory;
	}

	bool AllLayersAreInactive ()
	{
		return inactive;
	}
}