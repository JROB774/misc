/*******************************************************************************
 * A variety of different algorithms used to convert an image to greyscale.
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

#include "stb_image.h"
#include "stb_image_write.h"

typedef uint8_t u8;

// Change this value to change how many shades of grey are used in the
// functions: GreyscaleNumShadesSolid() and GreyscaleNumShadesDither()
constexpr int GREYSCALE_NUM_SHADES = 16;

// Most quick and simple averaging method (generally a bad/naive approach).
static inline void GreyscaleAverage          (u8* _data, int _w, int _h, int _bpp);
// Takes into account the weighted luminance values of different channels.
static inline void GreyscaleLuminance        (u8* _data, int _w, int _h, int _bpp);
// Desaturates the RGB (could be HSL) values -- usually has least contrast.
static inline void GreyscaleDesaturation     (u8* _data, int _w, int _h, int _bpp);
// Makes each pixel equal to the min/max out of their three RGB components.
static inline void GreyscaleDecompositionMax (u8* _data, int _w, int _h, int _bpp);
static inline void GreyscaleDecompositionMin (u8* _data, int _w, int _h, int _bpp);
// Converts each pixel to the value of the specific color channel specified.
static inline void GreyscaleChannelRed       (u8* _data, int _w, int _h, int _bpp);
static inline void GreyscaleChannelGreen     (u8* _data, int _w, int _h, int _bpp);
static inline void GreyscaleChannelBlue      (u8* _data, int _w, int _h, int _bpp);
// Generates a greyscale palette from different grey shades and applies it.
static inline void GreyscaleNumShadesSolid   (u8* _data, int _w, int _h, int _bpp);
static inline void GreyscaleNumShadesDither  (u8* _data, int _w, int _h, int _bpp);

static inline int MaxRGB (int _r, int _g, int _b)
{
    // Calculate the maximum value out of the three RGB channels.
    return ((_r > _g) ? ((_r > _b) ? _r : _b) : ((_g > _b) ? _g : _b));
}
static inline int MinRGB (int _r, int _g, int _b)
{
    // Calculate the minimum value out of the three RGB channels.
    return ((_r < _g) ? ((_r < _b) ? _r : _b) : ((_g < _b) ? _g : _b));
}

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

        // Replace this with any of the algorithms.
        GreyscaleNumShadesDither(data, width, height, bpp);

        char filename[1024] = {};
        strcat(filename, _argv[i]);
        strcat(filename, "_greyscale");
        strcat(filename, ".png");

        // Output a brand new greyscale version of the image as a PNG.
        int pitch = width * bpp;
        stbi_write_png(filename, width, height, bpp, data, pitch);

        stbi_image_free(data);
        data = NULL;
    }

    return 0;
}

static inline void GreyscaleAverage (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        int r = _data[i+0];
        int g = _data[i+1];
        int b = _data[i+2];

        u8 grey = (r + g + b) / 3;

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleLuminance (u8* _data, int _w, int _h, int _bpp)
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

static inline void GreyscaleDesaturation (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        int r = _data[i+0];
        int g = _data[i+1];
        int b = _data[i+2];

        u8 grey = (MaxRGB(r,g,b) + MinRGB(r,g,b)) / 2;

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleDecompositionMax (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        int r = _data[i+0];
        int g = _data[i+1];
        int b = _data[i+2];

        u8 grey = MaxRGB(r,g,b);

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleDecompositionMin (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        int r = _data[i+0];
        int g = _data[i+1];
        int b = _data[i+2];

        u8 grey = MinRGB(r,g,b);

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleChannelRed (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        u8 grey = _data[i+0];

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleChannelGreen (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        u8 grey = _data[i+1];

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleChannelBlue (u8* _data, int _w, int _h, int _bpp)
{
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        u8 grey = _data[i+2];

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleNumShadesSolid (u8* _data, int _w, int _h, int _bpp)
{
    int conversion_factor = 255 / (GREYSCALE_NUM_SHADES - 1);

    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        int r = _data[i+0];
        int g = _data[i+1];
        int b = _data[i+2];

        // This could use any greyscale algorithm but we will just use
        // the GreyscaleAverage() algorithm due to its simplistic result.
        int average_value = (r + g + b) / 3;

        u8 grey = (u8)((average_value / conversion_factor) + 0.5f) * conversion_factor;

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;
    }
}

static inline void GreyscaleNumShadesDither (u8* _data, int _w, int _h, int _bpp)
{
    int conversion_factor = 255 / (GREYSCALE_NUM_SHADES - 1);
    int error_value = 0;

    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        int r = _data[i+0];
        int g = _data[i+1];
        int b = _data[i+2];

        // This could use any greyscale algorithm but we will just use
        // the GreyscaleAverage() algorithm due to its simplistic result.
        u8 grey = (r + g + b) / 3;

        int temp_grey = grey + error_value;
        temp_grey = (int)((temp_grey / conversion_factor) + 0.5f) * conversion_factor;
        error_value = (grey + error_value) - temp_grey;
        grey = (u8)temp_grey;

        _data[i+0] = grey;
        _data[i+1] = grey;
        _data[i+2] = grey;

        if (i != 0 && ((i / _bpp) % _w) == 0) {
            error_value = 0;
        }
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
