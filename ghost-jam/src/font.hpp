#pragma once

struct Font
{
    float char_w;
    float char_h;

    std::string glyph_cache;

    Quad posistion[128];
};

STDDEF void load_font_from_file (Font& _font, const char* _file_name);
STDDEF void draw_bitmap_text (Font& _font, float _x, float _y, std::string _text);
