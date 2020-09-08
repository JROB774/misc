/*******************************************************************************
 * Generates a zoomed out image of the mandelbrot fractal set.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

// c++ library includes
#include <iostream>
#include <fstream>
// c library includes
#include <cstdlib>
#include <cstdint>

double MapToReal (int32_t _x, int32_t _width, double _minreal, double _maxreal)
{
    // maps a real number based on the integral range
    double range = _maxreal - _minreal;
    return (double)_x * (range / (double)_width) + _minreal;
}

double MapToImag (int32_t _y, int32_t _height, double _minimag, double _maximag)
{
    // maps an imaginary number based on the integral range
    double range = _maximag - _minimag;
    return (double)_y * (range / (double)_height) + _minimag;
}

int32_t FindMandelbrot (double _creal, double _cimag, int32_t _maxiter)
{
    int32_t i = 0;
    double zreal = 0.0, zimag = 0.0;

    // runs through the mandelbrot forumla
    while (i < _maxiter && zreal * zreal + zimag * zimag < 4.0) {
        double temp = zreal * zreal - zimag * zimag + _creal;
        zimag = 2.0 * zreal * zimag + _cimag;
        zreal = temp;
        ++i;
    }

    return i;
}

int32_t main (int32_t _argc, char* _argv[])
{
    int32_t width   =  512;
    int32_t height  =  512;
    int32_t maxnum  =  255;
    double  minreal = -1.5;
    double  maxreal =  0.7;
    double  minimag = -1.0;
    double  maximag =  1.0;

    // open and write the output ppm header
    std::fstream ppm("output.ppm", std::ios::out);
    if (!ppm.is_open()) { /* @TODO: handle error */ }
    ppm << "P3\n" << width << " " << height << "\n" << "256\n";

    // iterate through pixel grid
    for (int32_t iy = 0; iy < height; ++iy) {
        for (int32_t ix = 0; ix < width; ++ix) {
            // find real and imag values for c, corresponding to pixel
            double creal = MapToReal(ix, width, minreal, maxreal);
            double cimag = MapToImag(iy, height, minimag, maximag);

            // find the number of iterations in formula, using c
            int32_t n = FindMandelbrot(creal, cimag, maxnum);
            // map the resulting number to an RGB value
            int32_t r = (n % 256), g = (n % 256), b = (n % 256);

            // output the rgb to the ppm file
            ppm << r << " " << g << " " << b << " ";
        }
    }

    // close the ppm output file
    ppm.close();

    // successful exit
    return EXIT_SUCCESS;
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

