#pragma once

struct Anims
{
    float offset_x;
    float offset_y;
    float frame_width;
    float frame_height;

    std::vector<float> frames;
};

struct Anim_State
{
    int loops; //-1 infinite
    float time;
};

STDDEF void load_animation_from_file(Anims& _anims, const char* _file_name);
STDDEF void draw_texture_animated(Anims& _anims, Texture& _texture, float _pos_x, float _pos_y, Anim_State& _states);