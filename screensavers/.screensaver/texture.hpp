#ifndef TEXTURE_HPP__ /*//////////////////////////////////////////////////////*/
#define TEXTURE_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

typedef GLuint Texture_Handle;
typedef GLenum Texture_Wrap;

GLOBAL constexpr Texture_Wrap TEXTURE_WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER;
GLOBAL constexpr Texture_Wrap TEXTURE_WRAP_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE;
GLOBAL constexpr Texture_Wrap TEXTURE_WRAP_REPEAT          = GL_REPEAT;
GLOBAL constexpr Texture_Wrap TEXTURE_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT;

struct Texture
{
    float w;
    float h;

    Texture_Handle handle;
};

STDDEF bool load_texture_from_file (Texture& texture, const char* file_name);
STDDEF bool load_texture_from_mem  (Texture& texture, const void* buf, size_t sz);

STDDEF void free_texture           (Texture& texture);

FILDEF void set_texture_wrap       (Texture_Wrap wrap);

FILDEF void use_texture            (Texture& texture);

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* TEXTURE_HPP__ //////////////////////////////////////////////////////*/
