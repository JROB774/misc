namespace Asset
{
	constexpr int FONT_SIZE = 24;

	GPU_Image* tex_hotbar_buttons;
	GPU_Image* tex_tiles;
	GPU_Image* tex_align;

	FC_Font* fnt_standard;
	
	void Init ()
	{
		// TEXTURES: ///////////////////////////////////////////////////////////

		// if any of the files fail we will terminate
		bool failed = false;

		// load in all the textures needed for the program, globally accessible
		if (!Render::LoadTexture(tex_hotbar_buttons, DataManager::Load("buttons.png"))) { failed = true; }

		/*
		// @NOTE: EXTERNAL TILES REMOVED DUE TO THE MIPMAPPING ISSUES - MAY BE REIMPLEMENTED LATER
		
		// determine if external or internal tiles should be used
		bool external = Settings::GetBoolean("use_external_tile_graphics");
		if (external) {
			// if so get the path spcified and load the file
			std::string path = Settings::GetString("external_tile_graphics_path");
			SDL_RWops* stream = SDL_RWFromFile(path.c_str(), "r");
			if (!stream) { external = false; }
			if (!Render::LoadTexture(tex_tiles, stream)) { external = false; }
			// inform the user of the failure before falling pack
			if (!external) {
				ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_GPU, "Failed to load external tile graphics '" + path + "'!");
			}
		}

		// if we don't want external tiles or the load failed, go to internal
		if (!external) {
			// determine if old or new tile graphics are used
			std::string postfix;
			if (Settings::GetBoolean("use_old_tile_graphics")) { postfix = "_old.png"; }
			else { postfix = "_new.png"; }
			if (!Render::LoadTexture(tex_tiles, DataManager::Load("tiles" + postfix))) { failed = true; }
		}
		*/

		// determine if old or new tile graphics are used
		std::string postfix;
		if (Settings::GetBoolean("use_old_tile_graphics")) { postfix = "_old.png"; }
		else { postfix = "_new.png"; }
		if (!Render::LoadTexture(tex_tiles, DataManager::Load("tiles" + postfix))) { failed = true; }

		// load the texture that is rendered over the level resize alignment control/prompty
		if (!Render::LoadTexture(tex_align, DataManager::Load("align.png"))) { failed = true; }

		// if we failed during any of the textures then terminate
		if (failed) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_STD, "Cannot run due to texture failure!"); }

		// FONTS: //////////////////////////////////////////////////////////////

		// loads in the internal, or external, font (used for rendering category names, tool-tips, etc.)
		if (!Settings::GetBoolean("use_external_font")) { Render::LoadFont(fnt_standard, FONT_SIZE, DataManager::Load("tein.ttf")); }
		else {
			std::string path = Settings::GetString("external_font_path");
			SDL_RWops* stream = SDL_RWFromFile(path.c_str(), "r");
			bool failed = false;
			if (!stream) { failed = true; }
			else { failed = !Render::LoadFont(fnt_standard, FONT_SIZE, stream); }
			// if we fail to load an external font just fall-back to the internal font
			if (failed) {
				ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_TTF, "Failed to load external font '" + path + "'!");
				Render::LoadFont(fnt_standard, FONT_SIZE, DataManager::Load("tein.ttf"));
			}
		}

		// if neither an internal or external font was able to be loaded, fatal error
		if (!fnt_standard) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_TTF, "Failed to load either an internal, or external, font!"); }
	}

	void Quit ()
	{
		// free memory for all textures
		GPU_FreeImage(tex_hotbar_buttons);
		GPU_FreeImage(tex_tiles);
		GPU_FreeImage(tex_align);
		// free memory for all fonts
		FC_FreeFont(fnt_standard);
	}
}