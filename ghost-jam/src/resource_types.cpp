void Level_Resource::load (std::string file_name_)
{
    load_level(data, file_name_.c_str());
}
void Level_Resource::free ()
{
    // Nothing...
}

void Truetype_Resource::load (std::string file_name_)
{
    load_truetype_from_file(data, file_name_.c_str());
}
void Truetype_Resource::free ()
{
    free_truetype(data);
}

void Shader_Resource::load (std::string file_name_)
{
    data = load_shader_from_file(file_name_.c_str());
}
void Shader_Resource::free ()
{
    free_shader(data);
}

void Texture_Resource::load (std::string file_name_)
{
    load_texture_from_file(data, file_name_.c_str(), TEXTURE_WRAP_REPEAT);
}
void Texture_Resource::free ()
{
    free_texture(data);
}

void Font_Resource::load (std::string file_name_)
{
    load_font_from_file(data, file_name_.c_str());
}
void Font_Resource::free ()
{
    // Nothing...
}

void Anims_Resource::load(std::string file_name_)
{
    load_animation_from_file(data, file_name_.c_str());
}
void Anims_Resource::free()
{

}
