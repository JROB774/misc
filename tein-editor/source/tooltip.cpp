namespace Tooltip
{
	namespace
	{
		constexpr float NAME_SCALE = 1.f;
		constexpr float DESC_SCALE = .8f;

		constexpr float RADIUS = 6.f; // curve of the tooltip box

		constexpr int LINE_LENGTH = 64; // max line length, unless name is longer
		constexpr int TOP_GAP = 6; // added to the y pos of the tool-tip if being rendered above cursor

		constexpr int HORZ_PADDING = 6;
		constexpr int VERT_PADDING = 2;

		// the point size of each font/text
		constexpr int NAME_SIZE = Asset::FONT_SIZE;
		constexpr int DESC_SIZE = Asset::FONT_SIZE * DESC_SCALE;

		SDL_Color color;
		SDL_Color name_color;
		SDL_Color desc_color;

		std::string name;
		std::string desc;

		int width;
		int height;

		bool visible = false;
		bool enabled = false;
	}

	void Init ()
	{
		// sets the colors for the tooltip elements
		color = Settings::GetColor("tooltip_color");
		name_color = Settings::GetColor("tooltip_title_color");
		desc_color = Settings::GetColor("tooltip_text_color");

		// checks to see if tooltips are even desried
		enabled = Settings::GetBoolean("use_tooltips");
	}

	void Render ()
	{
		// only bother rendering if visisble
		if (!enabled || !visible) { return; }

		// we need to get the cursor position to begin rendering
		int x, y;
		SDL_GetMouseState(&x, &y);
		y += Cursor::SIZE;
		// determine if we need to render above the cursor (due to out-of-bounds)
		if ((y + height) > App::GetHeight()) {
			y -= (Cursor::SIZE + TOP_GAP + height);
		}

		// draw the background rectangle
		GPU_RectangleRoundFilled(App::screen, x-HORZ_PADDING, y-VERT_PADDING, x+width+HORZ_PADDING, y+height+VERT_PADDING, RADIUS, color);

		// draw the name and description text
		Render::DrawFont(x, y, name, Asset::fnt_standard, NAME_SCALE, NAME_SCALE, name_color);
		Render::DrawFont(x, y+NAME_SIZE, desc, Asset::fnt_standard, DESC_SCALE, DESC_SCALE, desc_color);
	}

	void Set (std::string _name, std::string _desc)
	{
		// if a tooltip is already set, don't create again
		if (!enabled || visible) { return; }
		visible = true;

		// sets the text fields for the tooltip
		name = _name;
		desc = _desc;

		// format the description text into a neat block (prevents off-screen)
		int maxwidth = std::max<int>(LINE_LENGTH, name.length());
		int linecount = 1; // the initial line of the desc
		if (maxwidth < (int)desc.length()) { // no need to format if we're short enough
			std::size_t offset = 0;
			for (std::size_t pos = 0; pos != std::string::npos; pos = desc.find(' ', pos)) {
				if (maxwidth <= (int)(pos - offset)) {
					desc.at(pos) = '\n';
					offset = pos;
					linecount++;
				}
				pos++;
			}
		}

		// calculates the width and height of the tooltip boxs
		if (name.length() < desc.length()) { width = FC_GetWidth(Asset::fnt_standard, desc.c_str()) * DESC_SCALE; }
		else { width = FC_GetWidth(Asset::fnt_standard, name.c_str()) * NAME_SCALE; }
		height = ((name.empty()) ? 0 : NAME_SIZE) + ((desc.empty()) ? 0 : DESC_SIZE * linecount);
	}

	void Unset ()
	{
		// unsets the current tooltip
		visible = false;
	}
}