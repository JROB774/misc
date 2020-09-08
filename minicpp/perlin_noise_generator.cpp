/*******************************************************************************
 * Generates a perlin noise texture and displays it in an SDL2 window.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <SDL.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t    i8;
typedef int16_t  i16;
typedef int32_t  i32;

const i32 WIN_WIDTH = 640, WIN_HEIGHT = 480;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
bool active = false;

void Error (const std::string _msg)
{
    FILE* log = fopen("error.log", "a");
    if (log != nullptr) {
        fprintf(log, "%s (%s)\n", _msg.c_str(), SDL_GetError());
        fclose(log);
    }
    printf("ERROR! Check 'error.log'...");
    exit(EXIT_FAILURE);
}

struct Image
{
public:

    Image (): texture(nullptr), width(0), height(0), pixels(nullptr), pitch(0) {}
    Image (const i32 _w, const i32 _h): texture(nullptr), width(_w), height(_h), pixels(nullptr), pitch(0)
    {
        texture = SDL_CreateTexture(
            renderer,
            SDL_GetWindowPixelFormat(window),
            SDL_TEXTUREACCESS_STREAMING,
            width, height
        );
        if (texture == nullptr) { Error("Couldn't create image!"); }
    }

    ~Image ()
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        width = 0, height = 0;
        pixels = nullptr;
        pitch = 0;
    }

    void Lock ()
    {
        pixels = nullptr, pitch = 0;
        if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) { Error("Couldn't lock an image!"); }
    }
    void Unlock ()
    {
        SDL_UnlockTexture(texture);
        pixels = nullptr, pitch = 0;
    }

    i32 GetWidth () const { return width; }
    i32 GetHeight () const { return height; }

    void* GetPixels () { return pixels; }
    i32 GetPitch () const { return pitch; }

    SDL_Texture* GetTexture () { return texture; }

private:

    SDL_Texture* texture;
    i32 width, height;
    void* pixels;
    i32 pitch;
};

class Perlin
{
public:

    Perlin() = delete;
    ~Perlin() = delete;

    static void GenNoise (Image* _image, const u32 _seed)
    {
        permutation.resize(256);
        std::iota(permutation.begin(), permutation.end(), 0);
        std::default_random_engine engine(_seed);
        std::shuffle(permutation.begin(), permutation.end(), engine);
        permutation.insert(permutation.end(), permutation.begin(), permutation.end());

        _image->Lock();
        u32* pixels = (u32*)_image->GetPixels();

        for (i32 iy = 0; iy < _image->GetHeight(); ++iy) {
            for (i32 ix = 0; ix < _image->GetWidth(); ++ix) {
                i32 i = iy * _image->GetWidth() + ix;

                double x = (double)ix / (double)_image->GetWidth();
                double y = (double)iy / (double)_image->GetHeight();

                /* STANDARD */
                double n = Noise(10 * x, 10 * y, 0.8);
                /* WOOD GRAIN */
                // double n = 20 * Noise(x, y, 0.8);
                // n -= floor(n);
                /* DIAGONAL MIRROR */
                // double n1 = Noise(x * 10, y * 10, 0.8), n2 = Noise(y * 10, x * 10, 0.8);
                // double n = n1 - n2;

                u32 colour = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, floor(255 * n), floor(255 * n), floor(255 * n), 0xFF);
                pixels[i] = colour;
            }
        }
        _image->Unlock();
    }

private:

    static std::vector <i32> permutation;

    static double Fade (double _t)
    {
        return _t * _t * _t * (_t *  (_t * 6 - 15) + 10);
    }

    static double Lerp (double _t, double _a, double _b)
    {
        return _a + _t * (_b - _a);
    }

    static double Grad (i32 _hash, double _x, double _y)
    {
        i32 hash = _hash & 15;
        double u = (hash < 8) ? _x : _y, v = (hash < 4) ? _y : _x;
        return ((hash & 1) == 0 ? u : -u) + ((hash & 2) == 0 ? v : -v);
    }

    static double Noise (double _x, double _y, double _z)
    {
        i32 x = (i32)floor(_x) & 255;
        i32 y = (i32)floor(_y) & 255;
        i32 z = (i32)floor(_z) & 255;

        _x -= floor(_x);
        _y -= floor(_y);
        _z -= floor(_z);

        double u = Fade(_x);
        double v = Fade(_y);
        double w = Fade(_z);

        i32 a  = permutation.at(x) + y;
        i32 aa = permutation.at(a) + z;
        i32 ab = permutation.at(a + 1) + z;
        i32 b  = permutation.at(x + 1) + y;
        i32 ba = permutation.at(b) + z;
        i32 bb = permutation.at(b + 1) + z;

        double res = Lerp(w, Lerp(v, Lerp(u, Grad(permutation.at(aa), _x, _y), Grad(permutation.at(ba), _x - 1, _y)),
            Lerp(u, Grad(permutation.at(ab), _x, _y - 1), Grad(permutation.at(bb), _x - 1, _y - 1))),
            Lerp(v, Lerp(u, Grad(permutation.at(aa + 1), _x, _y), Grad(permutation.at(ba + 1), _x - 1, _y)),
            Lerp(u, Grad(permutation.at(ab + 1), _x, _y - 1), Grad(permutation.at(bb + 1), _x - 1, _y - 1))));

        return (res + 1.0) / 2.0;
    }
};

std::vector <i32> Perlin::permutation;

int main (int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { Error("Could not init SDL!"); }

    window = SDL_CreateWindow(
        "Perlin Noise",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_WIDTH, WIN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr) { Error("Could not create window!"); }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) { Error("Could not create renderer!"); }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    Image image(WIN_WIDTH, WIN_HEIGHT);

    active = true;
    while (active) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) { active = false; }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0) {
                    Perlin::GenNoise(&image, (u32)time(nullptr));
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, image.GetTexture(), nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
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
