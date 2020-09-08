/*******************************************************************************
 * Makes an image greyscale and maps the tones/depth to different hues.
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
#include <math.h>

#include "stb_image.h"
#include "stb_image_write.h"

typedef uint8_t u8;

// Representations of the different color spaces.
struct HSL { double h, s, l; };
struct RGB { u8     r, g, b; };

// Takes an image, converts it to greyscale, uses these greyscale values to
// represent depth and then maps these depths to different hue values.
static inline void ConvertDepthToColor (u8* _data, int _w, int _h, int _bpp);

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

        // Perform the mapping from depth to hue and convert the HSL values
        // back into RGB color space values so that they can be saved out.
        ConvertDepthToColor(data, width, height, bpp);

        char filename[1024] = {};
        strcat(filename, _argv[i]);
        strcat(filename, "_hue");
        strcat(filename, ".png");

        // Output a brand new hue-mapped version of the image as a PNG.
        int pitch = width * bpp;
        stbi_write_png(filename, width, height, bpp, data, pitch);

        stbi_image_free(data);
        data = NULL;
    }

    return 0;
}

static inline double MapRange (double _val, double _a1, double _a2, double _b1, double _b2)
{
    // Maps a value from the range (_a1,_a2) to a value in the range (_b1,_b2).
    return _b1 + ((_b2 - _b1) / (_a2 - _a1)) * (_val - _a1);
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

static inline double HueToRGB (double _p, double _q, double _t)
{
    if (_t <     0.0) { _t += 1.0; }
    if (_t >     1.0) { _t -= 1.0; }
    if (_t < 1.0/6.0) { return _p +  (_q - _p) * 6.0 * _t; }
    if (_t < 1.0/2.0) { return _q; }
    if (_t < 2.0/3.0) { return _p + (_q - _p) * (2.0/3.0 - _t) * 6.0; }

    return _p;
}

static inline void HSLtoRGB (RGB* _rgb, const HSL* _hsl)
{
    // A standard algorithm for converting HSL color into RGB color.

    double r, g, b;
    double h, s, l;
    double p, q;

    // Cache HSL values from 0 - 1.
    h = _hsl->h / 360.0;
    s = _hsl->s, l = _hsl->l;

    if (s == 0.0) {
        r = l;
        g = l;
        b = l;
    } else {
        q = ((l < 0.5) ? (l * (1.0 + s)) : (l + s - l * s));
        p = 2 * l - q;

        r = HueToRGB(p, q, h + 1.0/3.0);
        g = HueToRGB(p, q, h);
        b = HueToRGB(p, q, h - 1.0/3.0);
    }

    _rgb->r = (u8)floor(r * 255.0);
    _rgb->g = (u8)floor(g * 255.0);
    _rgb->b = (u8)floor(b * 255.0);
}

static inline void ConvertDepthToColor (u8* _data, int _w, int _h, int _bpp)
{
    // Ensure that the input image is greyscale.
    GreyscaleLuminance(_data, _w, _h, _bpp);

    HSL hsl; RGB rgb;
    for (int i=0; i<(_w*_h*_bpp); i+=_bpp) {
        // Maps the greyscale tone of the image to a hue value.
        // It doesn't matter if we use the R, G, or B channels.
        hsl.h = MapRange(_data[i+0], 0, 255, 0, 360);
        hsl.s = 1.0f;
        hsl.l = 0.5f;

        // Converts HSL to RGB so it can be saved.
        HSLtoRGB(&rgb, &hsl);

        _data[i+0] = rgb.r;
        _data[i+1] = rgb.g;
        _data[i+2] = rgb.b;
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
