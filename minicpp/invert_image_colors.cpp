/*******************************************************************************
 * Converts all the pixels of the desired image to their inverted color value.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stb_image.h"
#include "stb_image_write.h"

typedef uint8_t u8;

int main (int _argc, char* _argv[])
{
    for (int i=1; i<_argc; ++i) {
        // Load the image specified as RGBA using the STB image library.
        int width, height, bpp = 4;
        u8* data = (u8*)stbi_load(_argv[i], &width, &height, NULL, bpp);
        if (!data) {
            fprintf(stderr, "Couldn't load the image '%s'!\n", _argv[i]);
            continue;
        }

        // Performs the actual inversion operation on all pixels.
        for (int j=0; j<(width*height*bpp); j+=bpp) {
            data[j+0] = 0xFF - data[j+0];
            data[j+1] = 0xFF - data[j+1];
            data[j+2] = 0xFF - data[j+2];
        }

        char filename[1024] = {};
        strcat(filename, _argv[i]);
        strcat(filename, "_inverted");
        strcat(filename, ".png");

        // Output a brand new inverted version of the image as a PNG.
        int pitch = width * bpp;
        stbi_write_png(filename, width, height, bpp, data, pitch);

        stbi_image_free(data);
        data = NULL;
    }

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
