/*******************************************************************************
 * Tests for a system where FreeType 2 loaded glyphs are packed and cached.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL2/SDL.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint16_t u32;
typedef uint16_t u64;
typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

static constexpr const char* WIN_TITLE = "Freetype Caching - Test";
static constexpr int WIN_X = SDL_WINDOWPOS_CENTERED, WIN_Y = SDL_WINDOWPOS_CENTERED;
static constexpr int WIN_W = 800, WIN_H = 600;
static constexpr int WIN_FLAGS = SDL_WINDOW_SHOWN;

static FT_Library freetype_library;

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Event event;

constexpr int TTF_CACHE_SIZE = 1024;
constexpr int TTF_CHAR_COUNT = 128;

struct Glyph
{
    SDL_Rect bounds; // the clip for the glyph stored in the cache
    float x_advance; // how much to advance cursor by after drawing
};

struct Truetype
{
    SDL_Texture* glyph_cache;
    Glyph glyphs[TTF_CHAR_COUNT];

    int pixel_height; // the specified pixel height for the font to be cached at
    int max_glyph_height; // the height of the tallest glyph in the cache

    int ascent, descent;
    int line_gap;

    FT_Face font_face;
    bool has_kerning;
};

static Truetype* LoadTruetype (const char* _filename, int _pixel_height)
{
    Truetype* truetype = (Truetype*)calloc(1, sizeof(Truetype));
    if (truetype) {
        // use freetype to load the font data for later use and caching
        if (FT_New_Face(freetype_library, _filename, 0, &truetype->font_face)) {
            printf("Failed to load the font face '%s'!", _filename);
        } else {
            // sets the pixel width and height of characters (not guaranteed to be exactly this size)
            if (FT_Set_Pixel_Sizes(truetype->font_face, 0, _pixel_height)) {
                printf("Failed to set the height for the font face '%s'!", _filename);
            } else {
                // create the glyph cache texture at the desired size
                // note, that this stage could potentially be done later to make cache sizes fit better
                truetype->glyph_cache = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TTF_CACHE_SIZE, TTF_CACHE_SIZE);
                if (truetype->glyph_cache) {
                    SDL_SetTextureBlendMode(truetype->glyph_cache, SDL_BLENDMODE_BLEND);

                    // ensures all glyphs are cached at their respective y offset so that when scaled
                    // they do not have any misalignment issues that could occur from rounding
                    truetype->pixel_height = _pixel_height;
                    truetype->max_glyph_height = truetype->font_face->size->metrics.height / 64;

                    truetype->ascent = truetype->font_face->ascender / 16;
                    truetype->descent = truetype->font_face->descender / 16;
                    truetype->line_gap = truetype->font_face->height / 16;

                    truetype->has_kerning = FT_HAS_KERNING(truetype->font_face);

                    // pack all of the glyphs into the texture cache
                    FT_GlyphSlot glyph_slot = truetype->font_face->glyph;
                    int tallest_glyph_height = 0;
                    int cursor_x = 0, cursor_y = 0;
                    void* cache_data;
                    u8* pixels;
                    int pitch;

                    SDL_LockTexture(truetype->glyph_cache, NULL, &cache_data, &pitch);
                    pixels = (u8*)cache_data;

                    for (int i=0; i<TTF_CHAR_COUNT; ++i) {
                        // loads the glyph into memory for rendering
                        int glyph_index = FT_Get_Char_Index(truetype->font_face, (char)i);
                        if (FT_Load_Glyph(truetype->font_face, glyph_index, FT_LOAD_RENDER)) {
                            printf("Failed to load glyph for the character '%c'!\n", (char)i);
                            continue;
                        }

                        // gets the graphic for the glyph so we can cache it
                        FT_Bitmap* bitmap = &glyph_slot->bitmap;

                        // ensures that the glyph will fit into the cache
                        if (cursor_x + bitmap->width >= TTF_CACHE_SIZE) { cursor_x = 0, cursor_y += (truetype->max_glyph_height - truetype->descent); }
                        // in an actual implementation of this system instead of just printing a message at this point we could
                        // attempt to resize the font's glyph cache to make room for the remaining glyphs to be baked into it
                        if (cursor_y + (truetype->max_glyph_height - truetype->descent) >= TTF_CACHE_SIZE) { printf("The font will not fit in the cache!"); break; }

                        // caches the glyph data (clip bounds and x advance) for later use
                        truetype->glyphs[i].bounds = SDL_Rect { cursor_x, cursor_y, (int)bitmap->width, (truetype->max_glyph_height - truetype->descent) };
                        truetype->glyphs[i].x_advance = glyph_slot->advance.x >> 6;

                        if (bitmap->width > 0 || bitmap->rows > 0) {
                            // render the currently loaded glyph to the cache for later use
                            int pixel_advance = bitmap->pitch / bitmap->width;
                            for (int iy=0; iy<(int)bitmap->rows; ++iy) {
                                for (int ix=0; ix<(int)bitmap->pitch; ix+=pixel_advance) {
                                    // this section could be optimised heavily but we are just testing the system for now
                                    pixels[((((cursor_y+truetype->max_glyph_height)-glyph_slot->bitmap_top)+iy) * (TTF_CACHE_SIZE*4) + ((cursor_x+(ix/pixel_advance))*4))+0] = bitmap->buffer[iy * bitmap->pitch + ix];
                                    pixels[((((cursor_y+truetype->max_glyph_height)-glyph_slot->bitmap_top)+iy) * (TTF_CACHE_SIZE*4) + ((cursor_x+(ix/pixel_advance))*4))+1] = 0xFF;
                                    pixels[((((cursor_y+truetype->max_glyph_height)-glyph_slot->bitmap_top)+iy) * (TTF_CACHE_SIZE*4) + ((cursor_x+(ix/pixel_advance))*4))+2] = 0xFF;
                                    pixels[((((cursor_y+truetype->max_glyph_height)-glyph_slot->bitmap_top)+iy) * (TTF_CACHE_SIZE*4) + ((cursor_x+(ix/pixel_advance))*4))+3] = 0xFF;
                                }
                            }
                        }

                        // advance the cursor for caching the next glyph
                        cursor_x += truetype->glyphs[i].bounds.w;
                    }

                    SDL_UnlockTexture(truetype->glyph_cache);
                }
            }
        }
    }

    return truetype;
}

static void FreeTruetype (Truetype* _truetype)
{
    if (_truetype) {
        SDL_DestroyTexture(_truetype->glyph_cache);
        free(_truetype);
    }
}

static void RenderTruetype (Truetype* _truetype, int _x, int _y, float _scale, const char* _text, SDL_Color _color)
{
    int x = _x;
    int y = _y;

    int glyph_index = 0;
    int prev_glyph_index = 0;

    for (const char* c=_text; *c; ++c) {
        glyph_index = FT_Get_Char_Index(_truetype->font_face, *c);
        if (_truetype->has_kerning && prev_glyph_index && glyph_index) {
            FT_Vector kerning_delta;
            // no need to check for errors because delta will simply be set to (0,0)
            FT_Get_Kerning(_truetype->font_face, prev_glyph_index, glyph_index, FT_KERNING_DEFAULT, &kerning_delta);
            x += kerning_delta.x >> 6;
        }

        switch (*c) {
            case ('\n'): case ('\r'): { x = 0, y += (_truetype->max_glyph_height * _scale); } break;
            case ('\t'): { x += (int)((float)_truetype->glyphs[(int)' '].x_advance * _scale) * 4; } break;

            default: {
                Glyph* glyph = &_truetype->glyphs[(int)*c];

                SDL_Rect dst = { x, y, (int)((float)glyph->bounds.w * _scale), (int)((float)glyph->bounds.h *_scale) };
                SDL_RenderCopy(renderer, _truetype->glyph_cache, &glyph->bounds, &dst);

                x += (int)((float)glyph->x_advance * _scale);
            } break;
        }

        // store this so that kerning can be applied to the next glyph
        prev_glyph_index = glyph_index;
    }
}

static void OutputTextureAsPNG (const char* _filename, SDL_Texture* _tex)
{
    u8* pixels = NULL;
    int width, height, pitch, bpp;

    SDL_QueryTexture(_tex, NULL, NULL, &width, &height);
    SDL_LockTexture(_tex, NULL, (void**)&pixels, &pitch);

    bpp = pitch / width; // determine the bytes-per-pixel

    stbi_write_png(_filename, width, height, bpp, pixels, pitch);

    SDL_UnlockTexture(_tex);
}

int main (int _argc, char* _argv[])
{
    if (_argc < 2) {
        printf("Supply a font for rendering!");
        return 0;
    }

    // more descriptive names for these variables when reading
    const char* font_filename = _argv[1];
    const char* text_to_render = "Entity";

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(WIN_TITLE, WIN_X, WIN_Y, WIN_W, WIN_H, WIN_FLAGS);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    FT_Init_FreeType(&freetype_library);

    Truetype* truetype = LoadTruetype(font_filename, 72.0f);
    OutputTextureAsPNG("cache.png", truetype->glyph_cache);

    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_RenderClear(renderer);
        RenderTruetype(truetype, 0, 0, 1.0f, text_to_render, SDL_Color { 0xFF,0xFF,0xFF,0xFF });
        SDL_RenderPresent(renderer);
    }

    FreeTruetype(truetype);

    FT_Done_FreeType(freetype_library);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/
