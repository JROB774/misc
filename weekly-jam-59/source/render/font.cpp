/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void ParseFontGlyphSize (Font& _font, Tokenizer& _tokenizer)
{
	_font.glyph_width = TokenToInteger(_tokenizer);
	_font.glyph_height = TokenToInteger(_tokenizer);
}

INLDEF String ParseFontGlyphCache (Tokenizer& _tokenizer)
{
	return TokenToString(_tokenizer);
}

STDDEF void LoadFontFromFile_Internal (FontHandle& _font_handle, const char* _filename)
{
	// If we don't want to reload a font specified by a specific handle then
	// assign the invalid handle a value (at the end of the font list).
	if (_font_handle == INVALID_ASSET_HANDLE) {
		_font_handle = font_list.count;
		font_list.AddElement(Font());
	}

	// Free the old font data to avoid memory leaks.
	Font& font = font_list[_font_handle];
	FreeTexture(font.glyph_cache);

	// Split the entire font file into lines for easy parsing.
	Array<String> lines = ReadEntireFileIntoLines(_filename);
	if (!lines.data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Failed to load font '%s'!", _filename);
		return;
	}

	// Parse the font file data into something usable by the game code.
	String glyph_cache_filename;
	Tokenizer tokenizer = {0};
	tokenizer.delim = " \n\r\t\v\f";
	for (size_t i=0; i<lines.count; ++i) {
		tokenizer.cursor = EatLeadingDelimiters(lines[i].c_string, tokenizer.delim);
		if (tokenizer.cursor[0] == '\0' || tokenizer.cursor[0] == '#') { // Ignore blanks and comments.
			continue;
		} else {
			GetToken(tokenizer);
			// Read for known font file keywords to get the data.
			if (strncmp(tokenizer.token, "glyph_size", tokenizer.token_length) == 0) {
				ParseFontGlyphSize(font, tokenizer);
			} else if (strncmp(tokenizer.token, "glyph_cache", tokenizer.token_length) == 0) {
				glyph_cache_filename = ParseFontGlyphCache(tokenizer);
			} else {
				TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Unknown keyword '%.*s' on line %d in font '%s'!",
					tokenizer.token_length, tokenizer.token, i, _filename);
			}
		}
	}

	// Combine the texture glyph path with the filename.
	String full_glyph_cache_filename = TEXTURE_PATH;
	full_glyph_cache_filename += glyph_cache_filename;

	// Go through and split the glyph cache into individual glyphs.
	LoadTextureFromFile_Internal(font.glyph_cache, full_glyph_cache_filename.c_string);
	if (font.glyph_cache == INVALID_ASSET_HANDLE) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Failed to load font glyph cache '%s'!", full_glyph_cache_filename.c_string);
	} else {
		// Goes through the loaded glyph cache and cuts it into individual glyphs.
		const Texture& glyph_cache = texture_list[font.glyph_cache];
		int cursor_x = 0, cursor_y = 0;

		for (size_t i=0; i<Font::GLYPH_COUNT; ++i) {
			font.glyphs[i] = { cursor_x, cursor_y, font.glyph_width, font.glyph_height };
			// Moves to the right, if at the edge go to the next row.
			cursor_x += font.glyph_width;
			if (cursor_x >= glyph_cache.width) {
				cursor_y += font.glyph_height;
				cursor_x = 0;
			}
		}

		// Register the asset with the hotloader, if not already registered.
		RegisterAssetForHotloading(_filename, ASSET_FONT, _font_handle);
	}
}

INLDEF FontHandle LoadFontFromFile (const char* _filename)
{
	FontHandle font_handle = INVALID_ASSET_HANDLE;
	LoadFontFromFile_Internal(font_handle, _filename);

	return font_handle;
}

INLDEF void RenderText (FontHandle _font_handle, const char* _text, int _x, int _y, float _scale_x, float _scale_y, float _angle, int _anchor_x, int _anchor_y, SDL_RendererFlip _flip)
{
	// Do not bother handling invalid strings.
	if (!_text) { return; }

	// Do not bother handling invalid font handles.
	if (_font_handle >= font_list.count) { return; }
	const Font& font = font_list[_font_handle];

	int cursor_x = _x;
	int cursor_y = _y;

	int scaled_width = CAST(int, CAST(float, font.glyph_width) * _scale_x);
	int scaled_height = CAST(int, CAST(float, font.glyph_height) * _scale_y);

	// Go through glyph-by-glyph and render to the screen.
	for (const char* c=_text; *c; ++c) {
		switch (*c) {
			// SPECIAL CHARACTERS
			case ('\n'):
			case ('\r'): { cursor_x  = _x, cursor_y += scaled_height; } break;
			case ('\t'): { cursor_x += (Font::TAB_LENGTH_IN_SPACES * scaled_width); } break;
			// STANDARD CHARACTERS
			default: {
				int ascii = CAST(int, *c);
				RenderTexture(font.glyph_cache, cursor_x, cursor_y, _scale_x, _scale_y,
					_angle, _anchor_x, _anchor_y, _flip, &font.glyphs[ascii]);
				cursor_x += scaled_width;
			} break;
		}
	}
}

INLDEF void SetFontColor (FontHandle _font_handle, const SDL_Color& _color)
{
	// Do not bother handling invalid font handles.
	if (_font_handle >= font_list.count) { return; }
	Font& font = font_list[_font_handle];

	SetTextureColor(font.glyph_cache, _color);
}

INLDEF int GetFontGlyphWidth (FontHandle _font_handle)
{
	// Do not bother handling invalid font handles.
	if (_font_handle >= font_list.count) { return 0; }
	Font& font = font_list[_font_handle];

	return font.glyph_width;

}
INLDEF int GetFontGlyphHeight (FontHandle _font_handle)
{
	// Do not bother handling invalid font handles.
	if (_font_handle >= font_list.count) { return 0; }
	Font& font = font_list[_font_handle];

	return font.glyph_height;
}

INLDEF int GetTextWidth (FontHandle _font_handle, const char* _text)
{
	// Do not bother handling invalid font handles.
	if (_font_handle >= font_list.count) { return 0; }
	Font& font = font_list[_font_handle];

	int text_width = 0;
	for (const char* c=_text; *c; ++c) {
		if (*c == '\t') { text_width += (font.glyph_width * Font::TAB_LENGTH_IN_SPACES); }
		else { text_width += font.glyph_width; }
	}
	return text_width;
}
INLDEF int GetTextHeight (FontHandle _font_handle, const char* _text)
{
	// Do not bother handling invalid font handles.
	if (_font_handle >= font_list.count) { return 0; }
	Font& font = font_list[_font_handle];

	int text_height = font.glyph_height;
	for (const char* c=_text; *c; ++c) {
		if (*c == '\n') { text_height += font.glyph_height; }
	}
	return text_height;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
