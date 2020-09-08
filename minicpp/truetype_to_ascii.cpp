/*******************************************************************************
 * Converts text into an ASCII art representation using a TTF font face.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>

#include "stb_truetype.h"

struct GlyphBitmap
{
    int width, height;
    unsigned char* data;
};

int main (int _argc, char* _argv[])
{
    // POTENTIAL IMPROVEMENTS THAT COULD BE MADE IN THE FUTURE:
    //
    // - Handle other metrics, such as ascent, descent, etc.
    // - Implement kerning for a more control over spacing.
    // - Handle other special characters (not just space key).
    // - Optimise the glyph loading and outputting process.
    // - Proper error handling and improvements to stability.
    //

    // We require a TTF file to use for the rendering, some text
    // that we will render as ASCII, and the size to render at.
    if (_argc != 4) { return 0; }

    FILE* ttf = fopen(_argv[1], "rb");

    fseek(ttf, 0L, SEEK_END);
    size_t size = ftell(ttf);
    rewind(ttf);

    size_t buffer_size = size * sizeof(unsigned char);
    unsigned char* buffer = (unsigned char*)malloc(buffer_size);
    fread(buffer, 1, size, ttf);

    fclose(ttf);
    ttf = NULL;

    int font_offset = stbtt_GetFontOffsetForIndex(buffer, 0);
    stbtt_fontinfo font_info;
    stbtt_InitFont(&font_info, buffer, font_offset);

    int pixel_height = atoi(_argv[3]);
    float font_scale = stbtt_ScaleForPixelHeight(&font_info, pixel_height);

    int length = strlen(_argv[2]);
    int max_glyph_height = 0;

    size_t bitmaps_size = length * sizeof(GlyphBitmap);
    GlyphBitmap* bitmaps = (GlyphBitmap*)malloc(bitmaps_size);
    for (int i=0; i<length; ++i) {
        bitmaps[i].data = stbtt_GetCodepointBitmap(&font_info, 0, font_scale,
            _argv[2][i], &bitmaps[i].width, &bitmaps[i].height, NULL, NULL);
        // Determine the tallest bitmap out of all the glyphs.
        if (max_glyph_height < bitmaps[i].height) {
            max_glyph_height = bitmaps[i].height;
        }
    }

    // Store the space glyph advance in case it is needed when rendering.
    int space_advance = 0;
    stbtt_GetCodepointHMetrics(&font_info, ' ', &space_advance, NULL);
    space_advance *= font_scale;

    // Go through the top row of all characters, then the next, etc.
    for (int y=0; y<max_glyph_height; ++y) {
        for (int i=0; i<length; ++i) {
            // Special case for the space glyph as it has no bitmap.
            if (_argv[2][i] == ' ') {
                for (int x=0; x<space_advance; ++x) { putchar(' '); }
                continue;
            }

            unsigned char* pixels = bitmaps[i].data;
            int diff_height = max_glyph_height - bitmaps[i].height;
            for (int x=0; x<bitmaps[i].width; ++x) {
                if (y < diff_height) { putchar(' '); continue; }
                int index = (y - diff_height) * bitmaps[i].width + x;
                putchar(" .-+=^*!?I30%#$@"[pixels[index] / 16]);
            }

            int advance = 0;
            stbtt_GetCodepointHMetrics(&font_info, _argv[2][i], &advance, NULL);
            advance *= font_scale;
            int diff_width = advance - bitmaps[i].width;
            for (int x=0; x<diff_width; ++x) { putchar(' '); }
        }
        putchar('\n');
    }

    for (int i=0; i<length; ++i) {
        stbtt_FreeBitmap(bitmaps[i].data, NULL);
        bitmaps[i].data = NULL;
    }
    free(bitmaps);
    bitmaps = NULL;

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
