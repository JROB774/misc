/*******************************************************************************
 * Small program to convert images into greyscale ASCII art text files.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stdint.h>
#include <stdio.h>

#include "stb_image.h"

typedef uint8_t u8;

static inline void Greyscale (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        float r = (float)_data[i+0] * 0.30f;
        float g = (float)_data[i+1] * 0.59f;
        float b = (float)_data[i+2] * 0.11f;

        u8 grey = (u8)(r + g + b);

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

int main (int _argc, char* _argv[])
{
    const char* SYMBOLS = "@$#%0£I/!*^;:,. ";
    char* ascii = NULL;

    for (int i=1; i<_argc; ++i) {
        // Load the image specified as RGBA using the STB image library.
        int width, height, bpp = 4;
        u8* data = (u8*)stbi_load(_argv[i], &width, &height, NULL, bpp);
        if (!data) {
            fprintf(stderr, "Couldn't load the image '%s'!\n", _argv[i]);
            continue;
        }

        // Convert to greyscale so we can use the image's brightness values
        // for calculating the ASCII symbol that should be used instead.
        Greyscale(data, width, height, bpp);

        // Create the buffer that will store the ASCII version of the image.
        size_t size = ((width * height) + (height - 1));
        ascii = (char*)realloc(ascii, size * sizeof(char));
        if (!ascii) {
            fprintf(stderr, "Couldn't create text for '%s'!\n", _argv[i]);
        } else {
            int column_counter = 0;
            for (int k=0,j=0; j<(width*height*bpp); ++k,j+=4) {
                // When we hit a new row of pixels we add a newline.
                if (column_counter == width) {
                    ascii[k++] = '\n';
                    column_counter = 0;
                }
                ascii[k] = SYMBOLS[data[j] / strlen(SYMBOLS)];
                ++column_counter;
            }

            char filename[1024] = {};
            strcat(filename, _argv[i]);
            strcat(filename, ".txt");

            FILE* output = fopen(filename, "wb");
            if (!output) {
                fprintf(stderr, "Couldn't save file '%s'!\n", filename);
            } else {
                fwrite(ascii, size, 1, output);
                fclose(output);
            }
        }

        stbi_image_free(data);
        data = NULL;
    }

    free(ascii);
    ascii = NULL;
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
