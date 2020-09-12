
STDDEF void load_animation_from_file(Anims& _animation, const char* _file_name)
{
    const GonObject& gon = load_gon_and_remove_comments(_file_name);
    //if(gon == FieldType::NULLGON){printf("GON ERROR\n");}

    _animation.offset_x = static_cast<float>(gon["offset_x"].Number());
    _animation.offset_y = static_cast<float>(gon["offset_y"].Number());
    _animation.frame_width = static_cast<float>(gon["frame_width"].Number());
    _animation.frame_height = static_cast<float>(gon["frame_height"].Number());

    for(int i = 0; i < gon["frames"].size(); i++){
        float temp = static_cast<float>(gon["frames"][i].Number());
        _animation.frames.push_back(temp);
    }
}

STDDEF void draw_texture_animated(Anims& _anims, Texture& _texture, float _pos_x, float _pos_y, Anim_State& _states)
{
    int current_frame = 0;
    float temp_state_time = _states.time;


    while(temp_state_time > _anims.frames.at(current_frame)){
        if(current_frame == _anims.frames.size() - 1){
            if(_states.loops == -1){ // Infinite Loop
                current_frame = 0;
                temp_state_time = 0;
                _states.time = 0;
                break;
            }
            else if(_states.loops == 0){break;} //Play Once
            else if(_states.loops > 0){ //Multiple Plays
                current_frame = 0;
                temp_state_time = 0;
                _states.time = 0;
                _states.loops--;
                break;
            }
        }
        else{
            temp_state_time -= _anims.frames.at(current_frame);
            current_frame++;
        }
    }

    // printf("%i\n", current_frame);
    Quad temp_quad = {_anims.offset_x + (_anims.frame_width * current_frame), _anims.offset_y, _anims.frame_width, _anims.frame_height};
    draw_texture(_texture, _pos_x, _pos_y, &temp_quad);
}
