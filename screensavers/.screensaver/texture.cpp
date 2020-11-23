/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

STDDEF bool impl__create_texture (Texture& texture, u8* pixels, int w, int h, int bpp)
{
    glGenTextures(1, &texture.handle);
    glBindTexture(GL_TEXTURE_2D, texture.handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (bpp != 3 && bpp != 4) return false;

    // Data loaded in from TGA files will be in BGRA order.
    GLenum format = 0;
    switch (bpp)
    {
    case (3): format = GL_BGR;  break;
    case (4): format = GL_BGRA; break;
    }

    GLint   level_of_detail = 0;
    GLint   internal_format = GL_RGBA;
    GLsizei texture_width   = w;
    GLsizei texture_height  = h;
    GLint   border          = 0; // Docs say must be zero!
    GLenum  type            = GL_UNSIGNED_BYTE;

    glTexImage2D(GL_TEXTURE_2D, level_of_detail, internal_format,
      texture_width, texture_height, border, format, type, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    texture.w = CAST(float, w);
    texture.h = CAST(float, h);

    return true;
}

/* -------------------------------------------------------------------------- */

STDDEF bool load_texture_from_file (Texture& texture, const char* file_name)
{
    int w, h, bpp;
    u8* pixels = load_tga_from_file(file_name, &w,&h,&bpp);
    if (!pixels) return false;
    defer { free_tga(pixels); };

    return impl__create_texture(texture, pixels, w, h, bpp);
}

STDDEF bool load_texture_from_mem  (Texture& texture, const void* buf, size_t sz)
{
    int w, h, bpp;
    u8* pixels = load_tga_from_mem(buf, sz, &w,&h,&bpp);
    if (!pixels) return false;
    defer { free_tga(pixels); };

    return impl__create_texture(texture, pixels, w, h, bpp);
}

STDDEF void free_texture (Texture& texture)
{
    glDeleteTextures(1, &texture.handle);

    texture.w = 0.0f;
    texture.h = 0.0f;
}

FILDEF void set_texture_wrap (Texture_Wrap wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

FILDEF void use_texture (Texture& texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.handle);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/
