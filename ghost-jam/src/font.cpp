
STDDEF void load_font_from_file (Font& _font, const char* _file_name)
{
    // @Incomplete: This should get the GON from resource manager but there is a bug right now...
    const GonObject& gon = load_gon_and_remove_comments(font_catalog.build_path(_file_name));

    _font.char_w     = static_cast<float>(gon["char_w"].Number());
    _font.char_h     = static_cast<float>(gon["char_h"].Number());
    _font.glyph_cache = gon["glyph_cache"].String();

    Texture* texture_ptr = &font_catalog.GET_TEXTURE_RESOURCE(_font.glyph_cache.c_str());

    int index_width  = static_cast<int>(floor(texture_ptr->w / _font.char_w));
    int index_height = static_cast<int>(floor(texture_ptr->h / _font.char_h));

    for(int y = 0; y < index_height; y++){
    for(int x = 0; x < index_width ; x++){
        _font.posistion[y * index_width + x] = {
                                            x * _font.char_w,
                                            y * _font.char_h,
                                            _font.char_w,
                                            _font.char_h};
    }
    }
}

STDDEF void draw_bitmap_text (Font& _font, float _x, float _y, std::string _text)
{
    Texture* texture_ptr = &font_catalog.GET_TEXTURE_RESOURCE(_font.glyph_cache.c_str());

    float temp_x = _x;
    float temp_y = _y;

    Quad temp_quad;
    Vec2 temp_vector = get_texture_draw_scale();

    for(int i = 0; i < _text.length(); i++){
        if(_text.at(i) == '\n'){
            temp_x = _x;
            temp_y += _font.char_h * temp_vector.y;
        }
        else if(_text.at(i) == '\t'){
            temp_x += (_font.char_w * 4) * temp_vector.x;
        }
        else{
            temp_quad = _font.posistion[_text[i]];
            temp_quad.w *= temp_vector.x;
            temp_quad.h *= temp_vector.y;
            draw_texture(*texture_ptr, temp_x, temp_y, &_font.posistion[_text[i]]);
            temp_x += _font.char_w * temp_vector.x;
        }
    }
}
