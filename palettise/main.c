#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL.h>

void RGBtoXYZ (const uint8_t _r, const uint8_t _g, const uint8_t _b, double* _x, double* _y, double *_z)
{
    double r = _r / 255;
    double g = _g / 255;
    double b = _b / 255;

    r = (r > 0.04045)?(pow(((r + 0.055) / 1.055), 2.4)):(r / 12.92);
    g = (g > 0.04045)?(pow(((g + 0.055) / 1.055), 2.4)):(g / 12.92);
    b = (b > 0.04045)?(pow(((b + 0.055) / 1.055), 2.4)):(b / 12.92);

    r *= 100.0;
    g *= 100.0;
    b *= 100.0;

    *_x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    *_y = r * 0.2126 + g * 0.1752 + b * 0.0722;
    *_z = r * 0.0193 + g * 0.1192 + b * 0.9505;
}

void XYZtoLAB (const double _x, const double _y, const double _z, double* _l, double* _a, double* _b)
{
    *_l = 10 * sqrt(_y);
    *_a = 17.5 * (((1.02 * _x) - _y) / sqrt(_y));
    *_b = 7 * ((_y - (0.847 * _z))/ sqrt(_y));

    /*
    double x = _x /  95.047;
    double y = _y / 100.000;
    double z = _z / 108.883;

    x = (x > 0.008856)?(pow(x, (1 / 3))):((7.787 * x) + (16 / 116));
    y = (y > 0.008856)?(pow(y, (1 / 3))):((7.787 * y) + (16 / 116));
    z = (z > 0.008856)?(pow(z, (1 / 3))):((7.787 * z) + (16 / 116));

    *_l = (116 * y) - 16;
    *_a =  500 * (x - y);
    *_b =  200 * (y - z);
    */
}

/*
void XYZtoRGB (const double _x, const double _y, const double _z, uint8_t* _r, uint8_t* _g, uint8_t* _b)
{
    double x = (_x != 0.0)?(_x / 100.0):(0.0);
    double y = (_y != 0.0)?(_y / 100.0):(0.0);
    double z = (_z != 0.0)?(_z / 100.0):(0.0);

    double r = x *  3.2406 + y * -1.5372 + z * -0.4986;
    double g = x * -0.9689 + y *  1.8758 + z *  0.0415;
    double b = x *  0.0557 + y * -0.2040 + z *  1.0570;

    r = (r > 0.0031308)?(1.055 * (pow(r, (1.0 / 2.4))) - 0.055):(12.92 * r);
    g = (g > 0.0031308)?(1.055 * (pow(g, (1.0 / 2.4))) - 0.055):(12.92 * g);
    b = (b > 0.0031308)?(1.055 * (pow(b, (1.0 / 2.4))) - 0.055):(12.92 * b);

    *_r = r * 255;
    *_g = g * 255;
    *_b = b * 255;
}
*/

/*
void LABtoXYZ (const double _l, const double _a, const double _b, double* _x, double* _y, double* _z)
{
    double y = (_l + 16) / 116;
    double x =  _a / 500 + y;
    double z =  y - _b / 200;

    y = (pow(y, 3) > 0.008856)?(pow(y, 3)):((y - 16 / 116) / 7.787);
    x = (pow(x, 3) > 0.008856)?(pow(x, 3)):((x - 16 / 116) / 7.787);
    z = (pow(z, 3) > 0.008856)?(pow(z, 3)):((z - 16 / 116) / 7.787);

    *_x =  95.047 * x;
    *_y = 100.000 * y;
    *_z = 108.883 * z;
}
*/

double ColourDistance(const SDL_Colour _col1, const SDL_Colour _col2)
{
    long rmean = ((long)_col1.r + (long)_col2.r) / 2;

    long r = (long)_col1.r - (long)_col2.r;
    long g = (long)_col1.g - (long)_col2.g;
    long b = (long)_col1.b - (long)_col2.b;

    return sqrt((((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8));
}


enum colour_type
{
    COL_BLACK   = 0x00,
    COL_WHITE   = 0x01,
    COL_RED     = 0x02,
    COL_CYAN    = 0x03,
    COL_PURPLE  = 0x04,
    COL_GREEN   = 0x05,
    COL_BLUE    = 0x06,
    COL_YELLOW  = 0x07,
    COL_ORANGE  = 0x08,
    COL_BROWN   = 0x09,
    COL_L_RED   = 0x0A,
    COL_D_GREY  = 0x0B,
    COL_GREY    = 0x0C,
    COL_L_GREEN = 0x0D,
    COL_L_BLUE  = 0x0E,
    COL_L_GREY  = 0x0F,
    COL_TOTAL
};

int main (int argc, char* argv[])
{
    /*
    uint8_t ___r = 255, ___g = 0, ___b = 0;
    double __x, __y, __z, __l, __a, __b;

    RGBtoXYZ(___r, ___g, ___b, &__x, &__y, &__z);
    XYZtoLAB(__x, __y, __z, &__l, &__a, &__b);

    printf("RGB: %d %d %d\n", ___r, ___g, ___b);
    printf("XYZ: %f %f %f\n", __x, __y, __z);
    printf("LAB: %f %f %f\n", __l, __a, __b);
    */

    Uint32 PALETTE[COL_TOTAL];
    /*
    const Uint32 PALETTE[COL_TOTAL] = {

        0x000000ff,
        0xffffffff,
        0x984b43ff,
        0x79c1c8ff,
        0x9b51a5ff,
        0x68ae5cff,
        0x52429dff,
        0xc9d684ff,
        0x9b67c9ff,
        0x6a5400ff,
        0xc37b65ff,
        0x636363ff,
        0x8a8a8aff,
        0xa3e599ff,
        0x8a7bceff,
        0xadadadff
    };
    */

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Texture* texture = NULL;
    uint32_t width = 0, height = 0;
    void* pixels = NULL;
    int32_t pitch = 0;

    //INITIALISE////////////////////////////////////////////////////////////////

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Palette Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    PALETTE[COL_BLACK]   = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x00, 0x00, 0x00, 0xFF);
    PALETTE[COL_WHITE]   = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xFF, 0xFF, 0xFF, 0xFF);
    PALETTE[COL_RED]     = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x98, 0x4B, 0x43, 0xFF);
    PALETTE[COL_CYAN]    = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x79, 0xC1, 0xC8, 0xFF);
    PALETTE[COL_PURPLE]  = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x9B, 0x51, 0xA5, 0xFF);
    PALETTE[COL_GREEN]   = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x68, 0xAE, 0x5C, 0xFF);
    PALETTE[COL_BLUE]    = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x52, 0x42, 0x9D, 0xFF);
    PALETTE[COL_YELLOW]  = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xC9, 0xD6, 0x84, 0xFF);
    PALETTE[COL_ORANGE]  = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x9B, 0x67, 0x39, 0xFF);
    PALETTE[COL_BROWN]   = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x6A, 0x54, 0x00, 0xFF);
    PALETTE[COL_L_RED]   = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xC3, 0x7B, 0x75, 0xFF);
    PALETTE[COL_D_GREY]  = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x63, 0x63, 0x63, 0xFF);
    PALETTE[COL_GREY]    = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x8A, 0x8A, 0x8A, 0xFF);
    PALETTE[COL_L_GREEN] = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xA3, 0xE5, 0x99, 0xFF);
    PALETTE[COL_L_BLUE]  = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x8A, 0x7B, 0xCE, 0xFF);
    PALETTE[COL_L_GREY]  = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xAD, 0xAD, 0xAD, 0xFF);

    //CREATE_TEXTURE////////////////////////////////////////////////////////////

    SDL_Surface* surface = NULL;
    SDL_Surface* converted = NULL;

    surface = SDL_LoadBMP("PUT_AN_IMAGE_TO_LOAD_IN_HERE");
    converted = SDL_ConvertSurface(surface, SDL_GetWindowSurface(window)->format, 0);

    texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, converted->w, converted->h);

    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    memcpy(pixels, converted->pixels, converted->pitch * converted->h);

    width = converted->w;
    height = converted->h;

    SDL_UnlockTexture(texture);
    pixels = NULL;
    pitch = 0;

    //RESIZE_THE_WINDOW/////////////////////////////////////////////////////////

    SDL_SetWindowSize(window, width, height);

    //COLOUR_PALETTE_CONVERTER//////////////////////////////////////////////////

    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    Uint32* texpixels = (Uint32*)pixels;
    int64_t pixelCount = (pitch / 4) * height;

    for (int64_t i = 0; i < pixelCount; ++i) {
        bool allowed = false;
        for (int ii = 0; ii < COL_TOTAL; ++ii) {
            if (texpixels[i] == PALETTE[ii]) {
                allowed = true;
                break;
            }
        }

        if (allowed) { continue; }
        else {
            double differences[COL_TOTAL];

            SDL_Colour col;
            SDL_GetRGB(texpixels[i], SDL_GetWindowSurface(window)->format, &col.r, &col.g, &col.b);

            for (uint8_t ii = 0; ii < COL_TOTAL; ++ii) {
                SDL_Colour pal;
                SDL_GetRGB(PALETTE[ii], SDL_GetWindowSurface(window)->format, &pal.r, &pal.g, &pal.b);

                differences[ii] = ColourDistance(col, pal);
            }

            uint8_t lowestid = 0;

            for (uint8_t ii = 0; ii < COL_TOTAL; ++ii) {
                if (differences[ii] <= differences[lowestid]) { lowestid = ii; }
            }

            texpixels[i] = PALETTE[lowestid];

            /* THIRD ALGORITHM - REALLY BAD...
            double differences[COL_TOTAL];

            SDL_Colour col;
            SDL_GetRGB(texpixels[i], SDL_GetWindowSurface(window)->format, &col.r, &col.g, &col.b);

            // CONVERSION FROM RGB -> LAB!!!
            double cx, cy, cz;
            RGBtoXYZ(col.r, col.g, col.b, &cx, &cy, &cz);
            double cl, ca, cb;
            XYZtoLAB(cx, cy, cz, &cl, &ca, &cb);

            for (uint8_t ii = 0; ii < COL_TOTAL; ++ii) {
                SDL_Colour pal;
                SDL_GetRGB(PALETTE[ii], SDL_GetWindowSurface(window)->format, &pal.r, &pal.g, &pal.b);

                double px, py, pz;
                RGBtoXYZ(pal.r, pal.g, pal.b, &px, &py, &pz);
                double pl, pa, pb;
                XYZtoLAB(px, py, pz, &pl, &pa, &pb);

                differences[ii] = sqrt(pow((pl - cl), 2) + pow((pa - ca), 2) + pow((pb - cb), 2));
            }

            uint8_t lowestid = 1;

            for (uint8_t ii = 1; ii < COL_TOTAL; ++ii) {
                if (differences[ii] <= differences[lowestid]) { lowestid = ii; }
            }

            texpixels[i] = PALETTE[lowestid];
            */

            /* SECOND ALGORITHM - BETTER, JUST NEEDS MORE ACCURATE CONVERSION
            uint32_t differences[COL_TOTAL];

            SDL_Colour col;
            SDL_GetRGB(texpixels[i], SDL_GetWindowSurface(window)->format, &col.r, &col.g, &col.b);

            for (uint8_t ii = 0; ii < COL_TOTAL; ++ii) {
                SDL_Colour pal;
                SDL_GetRGB(PALETTE[ii], SDL_GetWindowSurface(window)->format, &pal.r, &pal.g, &pal.b);

                differences[ii] = sqrt((pal.r - col.r)^2 + (pal.g - col.g)^2 + (pal.b - col.b)^2);
            }

            uint8_t lowestid = 0;

            for (uint8_t ii = 0; ii < COL_TOTAL; ++ii) {
                if (differences[ii] <= differences[lowestid]) { lowestid = ii; }
            }

            texpixels[i] = PALETTE[lowestid];
            */

            /* FIRST ALOGIRTHM - DECENT BUT NOT THE BEST
            SDL_Colour differences[COL_TOTAL];

            SDL_Colour currentcolour;
            SDL_GetRGB(texpixels[i], SDL_GetWindowSurface(window)->format, &currentcolour.r, &currentcolour.g, &currentcolour.b);

            for (uint8_t ii = 0; ii < COL_TOTAL; ++ii) {
                SDL_Colour currentpalette;
                SDL_GetRGB(PALETTE[ii], SDL_GetWindowSurface(window)->format, &currentpalette.r, &currentpalette.g, &currentpalette.b);

                differences[ii].r = abs(currentpalette.r - currentcolour.r);
                differences[ii].g = abs(currentpalette.g - currentcolour.r);
                differences[ii].b = abs(currentpalette.b - currentcolour.r);
                differences[ii].a = 0xFF;
            }

            uint8_t lowestid = 1;
            uint32_t lowest[COL_TOTAL];

            for (uint8_t ii = 1; ii < COL_TOTAL; ++ii) {
                lowest[ii] = differences[ii].r + differences[ii].g + differences[ii].b;
                if (lowest[ii] <= lowest[lowestid]) { lowestid = ii; }
            }

            texpixels[i] = PALETTE[lowestid];
            */
        }
    }

    SDL_UnlockTexture(texture);
    pixels = NULL;
    pitch = 0;

    //COLOUR_KEY////////////////////////////////////////////////////////////////
    /*
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    Uint32* texpixels = (Uint32*)pixels;
    int32_t pixelCount = (pitch / 4) * height;

    Uint32 ckey = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0xFF, 0x00, 0xDC);
    Uint32 trans = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0x00, 0x00, 0x00, 0x00);

    for (int32_t i = 0; i < pixelCount; ++i) {
        if (texpixels[i] == ckey) {
            texpixels[i] = trans;
        }
    }

    SDL_UnlockTexture(texture);
    pixels = NULL;
    pitch = 0;
    */
    //RENDER////////////////////////////////////////////////////////////////////

    SDL_RenderClear(renderer);

    SDL_Rect src = { 0, 0, width, height };
    SDL_Rect dst = { 0, 0, width, height };

    SDL_RenderCopy(renderer, texture, &src, &dst);

    SDL_RenderPresent(renderer);

    //INPUT_LOOP////////////////////////////////////////////////////////////////

    SDL_Event event;
    bool quit = false;

    while (true) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        if (quit) { break; }
    }

    //TERMINATE/////////////////////////////////////////////////////////////////

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(converted);
    SDL_DestroyTexture(texture);

    SDL_Quit();

    return 0;
}
