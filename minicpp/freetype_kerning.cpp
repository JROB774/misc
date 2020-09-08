/*******************************************************************************
 * Testing out the FreeType 2 API for fonts and how to add kerning to text.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

// For this particular example the kerning does not fully work as old pixels
// will be overwritten (due to the fact we aren't using alpha for rendering).
//
// However, this clipping also proves that the kerning is working correctly.

#define _USE_MATH_DEFINES

#include <math.h>

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// modify these constexpr values to change the rendering of the text
static constexpr int         BUFFER_WIDTH  = 1024;
static constexpr int         BUFFER_HEIGHT = 1024;
static constexpr const char* FONT_TO_LOAD  = ""; // supply a font to use
static constexpr const char* TEXT_TO_DRAW  = "Entity... This is my text.";
static constexpr int         PIXEL_HEIGHT  = 32;
static constexpr int         POS_X         = 0;
static constexpr int         POS_Y         = 0;

static unsigned char buffer[BUFFER_WIDTH*BUFFER_HEIGHT];

static FT_Library freetype;
static FT_Face font_face;

static int freetype_error;

static inline void KerningRender (int _x, int _y);

int main (int _argc, char* _argv[])
{
    freetype_error = FT_Init_FreeType(&freetype);
    if (freetype_error) { printf("\nFailed to init FreeType!"); }

    freetype_error = FT_New_Face(freetype, FONT_TO_LOAD, 0, &font_face);
    if (freetype_error == FT_Err_Unknown_File_Format ) { printf("\nAttempted to load an unknown font format!"); }
    else if (freetype_error) { printf("\nFailed to load the desired font face!"); }

    // face, char width in 1/64th of points, char height in 1/64th of points, horz device res, vert device res
    // freetype_error = FT_Set_Char_Size(font_face, 0, 72*64, 72, 72);
    // if (freetype_error) { printf("\nFailed to set the char size of the font face!"); }\

    // sets the pixel width and height of characters (not guaranteed to be exactly this size)
    freetype_error = FT_Set_Pixel_Sizes(font_face, 0, PIXEL_HEIGHT);
    if (freetype_error) { printf("\nFailed to set the pixel character size of the font face!"); }

    // renders the text but also handles kerning for improved quality
    KerningRender(POS_X, POS_Y);

    stbi_write_png("output.png", BUFFER_WIDTH, BUFFER_HEIGHT, 1, buffer, BUFFER_WIDTH);

    FT_Done_Face(font_face);
    font_face = NULL;

    FT_Done_FreeType(freetype);
    freetype = NULL;

    return 0;
}

static inline void KerningRender (int _x, int _y)
{
    FT_Bool use_kerning = FT_HAS_KERNING(font_face);
    FT_GlyphSlot glyph_slot = font_face->glyph;
    FT_UInt previous = 0;

    int pen_x = _x;
    int pen_y = _y + (font_face->size->metrics.height / 64);

    for (const char* c=TEXT_TO_DRAW; *c; ++c) {
        int glyph_index = FT_Get_Char_Index(font_face, *c); // char-code to glyph index

        // retrieve kerning distance and move our pen position
        if (use_kerning && previous && glyph_index) {
            FT_Vector delta;
            // no need to check for errors because delta will simply be set to (0,0)
            FT_Get_Kerning(font_face, previous, glyph_index, FT_KERNING_DEFAULT, &delta);
            pen_x += delta.x >> 6;
        }

        // FT_LOAD_RENDER implies we want an anti-aliased glyph bitmap by default
        freetype_error = FT_Load_Glyph(font_face, glyph_index, FT_LOAD_RENDER);
        if (freetype_error) { continue; }

        FT_Bitmap* bitmap = &glyph_slot->bitmap;

        // output the text glyphs as pixels to a bitmap
        int x = pen_x; // + glyph_slot->bitmap_left;
        int y = pen_y - glyph_slot->bitmap_top;

        if (bitmap->pixel_mode == FT_PIXEL_MODE_GRAY) { // only handle 8-bit greyscale for now
            for (unsigned int i=0; i<bitmap->rows; ++i) {
                int destination = (y+i) * BUFFER_WIDTH + x;
                memcpy(buffer+destination, bitmap->buffer+(i*bitmap->width), bitmap->width);
            }
        }

        pen_x += glyph_slot->advance.x >> 6;

        // keep track of the previous glyph for proper kerniung
        previous = glyph_index;
    }
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
