namespace Render
{
	namespace Color
	{
		constexpr SDL_Color WHITE       = { 0xFF, 0xFF, 0xFF, 0xFF };
		constexpr SDL_Color BLACK       = { 0x00, 0x00, 0x00, 0xFF };
		constexpr SDL_Color GREY        = { 0x80, 0x80, 0x80, 0xFF };
		constexpr SDL_Color DARK_GREY   = { 0x40, 0x40, 0x40, 0xFF };
		constexpr SDL_Color RED         = { 0xFF, 0x00, 0x00, 0xFF };
		constexpr SDL_Color GREEN       = { 0x00, 0xFF, 0x00, 0xFF };
		constexpr SDL_Color BLUE        = { 0x00, 0x00, 0xFF, 0xFF };
		constexpr SDL_Color YELLOW      = { 0xFF, 0xFF, 0x00, 0xFF };
		constexpr SDL_Color ORANGE      = { 0xFF, 0x7F, 0x00, 0xFF };
		constexpr SDL_Color BROWN       = { 0x7F, 0x33, 0x00, 0xFF };
		constexpr SDL_Color PURPLE      = { 0x57, 0x00, 0x7F, 0xFF };
		constexpr SDL_Color MAGENTA     = { 0xFF, 0x00, 0xDC, 0xFF };
		constexpr SDL_Color CYAN        = { 0x00, 0xFF, 0xFF, 0xFF };
		constexpr SDL_Color TRANS_WHITE = { 0xFF, 0xFF, 0xFF, 0x00 };
		constexpr SDL_Color TRANS_BLACK = { 0x00, 0x00, 0x00, 0x00 };
	}

	namespace
	{
		GPU_Target* targetdefault = nullptr;
	}

	void SetDefaultTarget (GPU_Target* _target)
	{
		targetdefault = _target;
	}

	bool LoadTexture (GPU_Image*& _texture, SDL_RWops* _stream)
	{
		// will return the success state of the function
		bool successful = false;

		// loads a PNG file from memory, into a surface
		SDL_Surface* surface = GPU_LoadSurface_RW(_stream, true);
		if (!surface) { ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_GPU, "Failed to load PNG from memory!"); }
		else {
			// converts the surface into an accelerated usable GPU texture
			_texture = GPU_CopyImageFromSurface(surface);
			if (!_texture) { ERROR_LOG(Error::PRIORITY_MED, Error::TYPE_GPU, "Failed to convert PNG into texture!"); }
			else {
				// sets up the scaling filter for the texture
				GPU_GenerateMipmaps(_texture);
				GPU_SetImageFilter(_texture, GPU_FILTER_LINEAR_MIPMAP);
				// sets the wrapping mode (we want repeat for easy scaling of tilable elements)
				GPU_SetWrapMode(_texture, GPU_WRAP_REPEAT, GPU_WRAP_REPEAT);
				// texture has successfully loaded at this point
				successful = true;
			}
		}

		// deallocates surface memory, no longer needed
		SDL_FreeSurface(surface);
		surface = nullptr;

		// inform the caller of success
		return successful;
	}

	void LoadTexture (GPU_Image*& _texture, std::uint16_t _w, std::uint16_t _h)
	{
		// create a blank image at the specified size
		_texture = GPU_CreateImage(_w, _h, GPU_FORMAT_RGBA);
		if (!_texture) { ERROR_LOG(Error::PRIORITY_MAX, Error::TYPE_GPU, "Failed to create image!"); }
		// sets up the scaling filter for the texture
		GPU_SetImageFilter(_texture, GPU_FILTER_LINEAR_MIPMAP);
		// creates a target for the image (so stuff can be rendered to it)
		GPU_LoadTarget(_texture);
	}

	void DrawTexture (float _x, float _y, float _ax, float _ay, float _sx, float _sy, GPU_Image* _texture, GPU_Rect* _src, SDL_Color _color, GPU_Target* _target = nullptr)
	{
		// don't attempt to render a non-existent texture
		if (!_texture) { return; }

		// renders the texture with appropriate settings
		GPU_SetColor(_texture, _color);
		GPU_BlitTransformX(_texture, _src, ((!_target) ? targetdefault : _target), _x, _y, _ax, _ay, 0.f, _sx, _sy);
	}

	void DrawTexture (float _x, float _y, GPU_Image* _texture, GPU_Rect* _src, SDL_Color _color, GPU_Target* _target = nullptr)
	{
		DrawTexture(_x, _y, 0.f, 0.f, 1.f, 1.f, _texture, _src, _color, _target);
	}

	bool LoadFont (FC_Font*& _font, std::uint32_t _size, SDL_RWops* _stream)
	{
		// used to track the result of the function
		bool failed = false;

		// creates and loads a font from a TTF stored in memory/externally
		_font = FC_CreateFont();
		if (!_font) { ERROR_LOG(Error::PRIORITY_LOG, Error::TYPE_STD, "Failed to allocate font!"); failed = true; }
		else {
			if (!FC_LoadFont_RW(_font, _stream, 0, _size, Color::WHITE, TTF_STYLE_NORMAL)) {
				ERROR_LOG(Error::PRIORITY_LOG, Error::TYPE_GPU, "Failed to create font!");
				failed = true;
			}
			else { FC_SetFilterMode(_font, FC_FILTER_LINEAR); }
		}

		// deallocates RWops memory, no longer needed
		SDL_RWclose(_stream);
		_stream = nullptr;

		// return wether it was successful or not
		return !failed;
	}
	
	void DrawFont (float _x, float _y, std::string _text, FC_Font* _font, float _sx, float _sy, SDL_Color _color, GPU_Target* _target = nullptr)
	{
		// don't attempt to render a non-existent font
		if (!_font) { return; }

		// draws the font at the given position
		FC_Effect effect { FC_ALIGN_LEFT, { _sx, _sy }, _color };
		FC_DrawEffect(_font, ((!_target) ? targetdefault : _target), _x, _y, effect, _text.c_str());
	}
}