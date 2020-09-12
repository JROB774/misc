namespace internal
{
    GLOBAL Texture* current_texture;
}

FILDEF bool load_texture_from_data (Texture& _tex, const std::vector<u8>& _file_data, TextureWrap _wrap)
{
    const stbi_uc* buffer = &_file_data[0];
    int size = CAST(int, _file_data.size());

    int w, h, bpp;
    u8* raw_data = stbi_load_from_memory(buffer, size, &w, &h, &bpp, 0);
    if (!raw_data) {
        LOG_ERROR(ERR_MIN, "Failed to load texture from data!");
        return false;
    }
    defer { stbi_image_free(raw_data); };

    return create_texture(_tex, w, h, bpp, raw_data, _wrap);
}

FILDEF bool load_texture_from_file (Texture& _tex, const char* _file_name, TextureWrap _wrap)
{
    // Build an absolute path to the file based on the executable location.
    std::string file_name(make_path_absolute(_file_name));

    int w, h, bpp;
    u8* raw_data = stbi_load(file_name.c_str(), &w, &h, &bpp, 0);
    if (!raw_data) {
        LOG_ERROR(ERR_MIN, "Failed to load texture '%s'!", file_name.c_str());
        return false;
    }
    defer { stbi_image_free(raw_data); };

    return create_texture(_tex, w, h, bpp, raw_data, _wrap);
}

FILDEF void free_texture (Texture& _tex)
{
    glDeleteTextures(1, &_tex.handle);
}

STDDEF bool create_texture (Texture& _tex, int _w, int _h, int _bpp, void* _data, TextureWrap _wrap)
{
    // Bytes-per-pixel needs to be one of these otherwise we can't use.
    ASSERT(_bpp == 1 || _bpp == 2 || _bpp == 3 || _bpp == 4);

    // Cannot create a texture if no data is supplied to us to use!
    if (!_data) {
        LOG_ERROR(ERR_MIN, "No texture data passed for creation!");
        return false;
    }

    // The caller should always pass in pixel data that is RGBA ordered.
    // So it's just a matter of determinig how many components we have.
    GLenum format;
    switch (_bpp) {
    default:
    case(1): { format = GL_RED;  } break;
    case(2): { format = GL_RG;   } break;
    case(3): { format = GL_RGB;  } break;
    case(4): { format = GL_RGBA; } break;
    }

    // Standard process for creating a GPU accelerated texture using GL.
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &_tex.handle);
    glBindTexture(GL_TEXTURE_2D, _tex.handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint   level_of_detail = 0;
    GLint   internal_format = GL_RGBA;
    GLsizei texture_width   = _w;
    GLsizei texture_height  = _h;
    GLint   border          = 0; // Docs say must be zero!
    GLenum  type            = GL_UNSIGNED_BYTE;

    glTexImage2D(GL_TEXTURE_2D, level_of_detail, internal_format,
      texture_width, texture_height, border, format, type, _data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Cache useful information regarding the texture for later use.
    _tex.w = CAST(float, _w), _tex.h = CAST(float, _h);
    _tex.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    return true;
}

FILDEF void set_texture (Texture& _texture, GLenum _unit)
{
    internal::current_texture = &_texture;

    glActiveTexture(_unit);
    glBindTexture(GL_TEXTURE_2D, _texture.handle);
}

FILDEF Texture* get_texture ()
{
    return internal::current_texture;
}
