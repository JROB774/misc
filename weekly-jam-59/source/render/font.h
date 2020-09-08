/*******************************************************************************
 * FONT
 * Systems for loading, handling, and rendering monospace bitmap fonts.
 *
*******************************************************************************/

#ifndef __TCE_FONT_H__ /*/////////////////////////////////////////////////////*/
#define __TCE_FONT_H__

namespace TCE
{

GLOBAL constexpr const char* FONT_PATH = "data/fonts/";

struct Font
{
	static constexpr int GLYPH_COUNT = 127;
	static constexpr int TAB_LENGTH_IN_SPACES = 4;

	TextureHandle glyph_cache = INVALID_ASSET_HANDLE;
	SDL_Rect glyphs[GLYPH_COUNT] = {0};

	int glyph_width = 0;
	int glyph_height = 0;
};

GLOBAL Array<Font> font_list;

STDDEF void LoadFontFromFile_Internal (FontHandle& _font_handle, const char* _filename);
INLDEF FontHandle LoadFontFromFile (const char* _filename);

INLDEF void RenderText (FontHandle _font_handle, const char* _text, int _x, int _y, float _scale_x = 1.0f, float _scale_y = 1.0f, float _angle = 0.0f, int _anchor_x = 0, int _anchor_y = 0, SDL_RendererFlip _flip = SDL_FLIP_NONE);

INLDEF void SetFontColor (FontHandle _font_handle, const SDL_Color& _color);

INLDEF int GetFontGlyphWidth (FontHandle _font_handle);
INLDEF int GetFontGlyphHeight (FontHandle _font_handle);

INLDEF int GetTextWidth (FontHandle _font_handle, const char* _text);
INLDEF int GetTextHeight (FontHandle _font_handle, const char* _text);

} // TCE

#endif /* __TCE_FONT_H__ /////////////////////////////////////////////////////*/
