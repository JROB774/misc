namespace internal
{
    GLOBAL constexpr int TOTAL_TRUETYPE_GLYPH_COUNT = 128;
    GLOBAL FT_Library freetype;
}

struct Truetype_Glyph
{
    Vec2  bearing;
    Quad  bounds;
    float advance;
};

enum Truetype_Mode
{
    TRUETYPE_MODE_SMALL,
    TRUETYPE_MODE_LARGE,
};

// I am very lazy...
struct Truetype_Face
{
    Truetype_Glyph glyphs[internal::TOTAL_TRUETYPE_GLYPH_COUNT];

    FT_Face face;

    Texture cache;

    bool  has_kerning;
    float line_gap;
    Vec4  color;
};

struct Truetype
{
    // This is an internal variable only used if the font is being loaded
    // from data. This is because FT_New_Memory_Face does not copy over
    // the data passed into it so the pointer provided must remain valid
    // until we are done with the font face and finally call FT_Done_Face.
    std::vector<u8> data;

    Truetype_Mode mode = TRUETYPE_MODE_SMALL;

    Truetype_Face lg;
    Truetype_Face sm;
};

FILDEF bool  load_truetype_from_data (Truetype& _font, const std::vector<u8>& _file_data);
FILDEF bool  load_truetype_from_file (Truetype& _font, const char* _file_name);

FILDEF void  free_truetype           (Truetype& _font);

FILDEF float get_truetype_kerning    (const Truetype& _font, char _c, int& _i, int& _p);
FILDEF float get_truetype_tab_width  (const Truetype& _font);

FILDEF float get_glyph_advance       (const Truetype& _font, char _c, int& _i, int& _p);

INLDEF float get_text_width          (const Truetype& _font, const char* _text);
INLDEF float get_text_height         (const Truetype& _font, const char* _text);

#define get_text_width_scaled( _font, _text) (get_text_width( _font, _text) * get_font_draw_scale())
#define get_text_height_scaled(_font, _text) (get_text_height(_font, _text) * get_font_draw_scale())

FILDEF void set_truetype_color (Truetype& _font, Vec4 _color);
FILDEF Vec4 get_truetype_color (Truetype& _font);
