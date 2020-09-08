/*******************************************************************************
 * Testing raycast intersection tests with walls (line-based intersections).
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#include <SDL2/SDL.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

#define GLOBAL static
#define PERSISTENT static
#define INTERNAL static

template <typename T>
constexpr inline T todeg (const T _rad)
{
    return _rad * 180.0 / M_PI;
}
template <typename T>
constexpr inline T torad (const T _deg)
{
    return _deg * M_PI / 180.0;
}

template <typename T>
constexpr inline void swap (T* const _a, T* const _b)
{
    T _t = *_a;
    *_a  = *_b;
    *_b  =  _t;
}

template <typename T>
constexpr inline T min (const T _a, const T _b)
{
    return (_a < _b) ? _a : _b;
}
template <typename T>
constexpr inline T max (const T _a, const T _b)
{
    return (_a < _b) ? _b : _a;
}

GLOBAL constexpr int FPS = 60, TPF = FPS / 1000;
GLOBAL constexpr const char* WIN_TITLE = "Portal Engine";
GLOBAL constexpr int WIN_W = 640, WIN_H = 480;

GLOBAL SDL_Window* window;
GLOBAL SDL_Surface* buffer;
GLOBAL SDL_Event event;
GLOBAL bool running;

inline void putpixel (int _x, int _y, u32 _col)
{
    /* do not handle out-of-bounds draws */
    if (_x < 0 || _x >= buffer->w ||
        _y < 0 || _y >= buffer->h) {
        return;
    }

    u32* pixels = (u32*)buffer->pixels;
    pixels[_y * buffer->w + _x] = _col;
}

inline void putline (int _x1, int _y1, int _x2, int _y2, u32 _col)
{
    bool steep = false;
    if (abs(_x1-_x2) < abs(_y1-_y2)) {
        swap(&_x1, &_y1);
        swap(&_x2, &_y2);
        steep = true;
    }
    if (_x1 > _x2) {
        swap(&_x1, &_x2);
        swap(&_y1, &_y2);
    }
    int dx = _x2-_x1;
    int dy = _y2-_y1;
    int derror2 = abs(dy)*2;
    int error2 = 0;
    int y = _y1;
    for (int x=_x1; x<=_x2; ++x) {
        if (steep) { putpixel(y, x, _col); }
        else { putpixel(x, y, _col); }
        error2 += derror2;
        if (error2 >= dx) {
            y += ((_y2 > _y1) ? 1 : -1);
            error2 -= dx*2;
        }
    }
}

typedef struct __Line__
{
    float x1,y1, x2,y2;
    int id, portal;

} Line;

typedef struct __Sector__
{
    Line lines[32];
    int numlines;

} Sector;

constexpr Sector MAP_DATA[] =
{
    #if 1
    Sector // 0
    {{
    Line {  32.0f,  64.0f, 256.0f,  32.0f, 0, -1 },
    Line { 256.0f,  32.0f, 196.0f, 256.0f, 1, -1 },
    Line { 196.0f, 256.0f,  96.0f, 320.0f, 2,  1 }, // portal to sector 1
    Line {  96.0f, 320.0f,  32.0f,  64.0f, 3, -1 }
    }, 4 },
    Sector // 1
    {{
    Line {  96.0f, 320.0f, 196.0f, 256.0f, 2,  0 }, // portal to sector 0
    Line { 196.0f, 256.0f, 256.0f, 390.0f, 4, -1 },
    Line { 256.0f, 390.0f,  16.0f, 420.0f, 5, -1 },
    Line {  16.0f, 420.0f,  96.0f, 320.0f, 6, -1 }
    }, 4 }
    #else
    Sector // 0
    {{
    Line {  32.0f,  32.0f, 256.0f,  32.0f, 0, -1 },
    Line { 256.0f,  32.0f, 256.0f, 256.0f, 1, -1 },
    Line { 256.0f, 256.0f,  32.0f, 256.0f, 2, -1 },
    Line {  32.0f, 256.0f,  32.0f,  32.0f, 3, -1 }
    }, 4 }
    #endif
};
constexpr int MAP_SECTORS = sizeof(MAP_DATA) / sizeof(Sector);

inline bool lineisvert (Line _ln) { return (_ln.x1==_ln.x2); }
inline bool lineishorz (Line _ln) { return (_ln.y1==_ln.y2); }

inline float getlength (Line _ln)
{
    return sqrtf(powf((_ln.x2-_ln.x1),2)+powf((_ln.y2-_ln.y1),2));
}

inline float getslope (Line _ln)
{
    return (_ln.y2-_ln.y1)/(_ln.x2-_ln.x1);
}

inline bool pointinlinebounds (Line _ln, float _x, float _y)
{
    // determine if the point is within the bounds
    // @NOTE: the +/-0.01f makes it work well with horz/vert walls
    return ((_x+0.01f >= min(_ln.x1,_ln.x2)) && (_x-0.01f <= max(_ln.x1,_ln.x2)) &&
            (_y+0.01f >= min(_ln.y1,_ln.y2)) && (_y-0.01f <= max(_ln.y1,_ln.y2)));
}

inline bool intersect (Line _l1, Line _l2, float* _x, float* _y)
{
    // add some form of bounding box check at some point in the function
    // if their bounds never collide then there is no chance of an
    // intersection -- this could lead to a more optimised function

    float x = *_x;
    float y = *_y;

    // if the lines are parallel then there is no intersection ever
    if ((lineisvert(_l1) && lineisvert(_l2)) ||
        (lineishorz(_l1) && lineishorz(_l2))) {
        return false;
    }

    // if vertical, offset slightly so the algorithm works
    if (lineisvert(_l1)) { _l1.x2 += 0.01f; }
    if (lineisvert(_l2)) { _l2.x2 += 0.01f; }

    // get the slopes of each line
    float m1 = getslope(_l1);
    float m2 = getslope(_l2);

    // if the diff between slopes are zero, no intersection (parallel)
    if ((m1-m2)==0.0f) { return false; }

    // get the intersepct of the lines
    float c1 = _l1.y1-m1*_l1.x1;
    float c2 = _l2.y1-m2*_l2.x1;

    // calculate the intersection
    x = (c2-c1)/(m1-m2);
    y = m1*x+c1;

    // set the final values afterwards
    if (_x) { *_x = x; }
    if (_y) { *_y = y; }

    // if in bounds an intersection does occur
    return (pointinlinebounds(_l1, x, y) &&
            pointinlinebounds(_l2, x, y));
}

inline bool pointinsectorbounds (const Sector* _sec, float _x, float _y)
{
    // cast a ray from the point if the total number of intersections
    // is even then it is outside, otherwise if odd it is inside

    // count all the intersexctions
    Line ray = { _x, _y, _x, FLT_MAX };
    float dumx, dumy; // dummy variables for the intersect func
    int numintersect = 0;
    for (int i=0; i<_sec->numlines; ++i) {
        if (intersect(_sec->lines[i], ray, &dumx, &dumy)) {
            ++numintersect;
        }
    }
    // determine if even or odd (in or out)
    return ((numintersect % 2) != 0);
}

bool sectorcast (const Sector* _sec, Line _ray, float* _x, float* _y, int _entrance)
{
    for (int i=0; i<_sec->numlines; ++i) {
        if (intersect(_ray, _sec->lines[i], _x, _y)) {
            int id = _sec->lines[i].id;
            int portal = _sec->lines[i].portal;
            if (id != _entrance) { // don't collide with entrance
                if (portal == -1) { return true; }
                else {
                    return sectorcast(&MAP_DATA[portal], _ray, _x, _y, id);
                }
            }
        }
    }

    return false;
}

void UpdateFrame ()
{
    // if locking failed and it's required, skip the frame
    if (SDL_MUSTLOCK(buffer) && SDL_LockSurface(buffer) != 0) {
        fprintf(stderr, "WRN: Skipping frame (%s)!\n", SDL_GetError());
    } else {
        // fill the buffer with black pixels
        SDL_FillRect(buffer, NULL, 0xFF000000);

        // gets the current mouse position
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // determines the current sector the mouse is in
        int cursector = 0;
        for (int i=0; i<MAP_SECTORS; ++i) {
            if (pointinsectorbounds(&MAP_DATA[i], mx, my)) {
                cursector = i;
                break;
            }
        }

        // handles intersections and draws cast rays
        float angle = 0.0f;
        for (int i=1; i<=640; ++i) {
            angle += (360.0f / 640.0f);
            float x1 = (float)mx, y1 = (float)my;
            Line ln = { x1, y1, cosf(torad(angle))*320+x1, sinf(torad(angle))*320+y1 };
            float clipx, clipy;
            if (sectorcast(&MAP_DATA[cursector], ln, &clipx, &clipy, -1)) {
                ln.x2 = clipx;
                ln.y2 = clipy;
            }
            putline(ln.x1, ln.y1, ln.x2, ln.y2, 0xFFFF0000);
        }

        // draws the collision line data
        for (int i=0; i<MAP_SECTORS; ++i) {
            for (int j=0; j<MAP_DATA[i].numlines; ++j) {
                Line ln = MAP_DATA[i].lines[j];
                putline(ln.x1, ln.y1, ln.x2, ln.y2, 0xFFFFFFFF);
            }
        }

        // display updated buffer contents
        SDL_UnlockSurface(buffer);
        SDL_UpdateWindowSurface(window);
    }
}

int main (int _argc, char* _argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "ERR: Failed to init SDL (%s)!\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow(WIN_TITLE, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
        if (!window) {
            fprintf(stderr, "ERR: Failed to create window (%s)!\n", SDL_GetError());
        } else {
            buffer = SDL_GetWindowSurface(window);
            if (!buffer) {
                fprintf(stderr, "ERR: Failed to get window buffer (%s)!\n", SDL_GetError());
            } else {
                running = true;
                while (running) {
                    u32 start = SDL_GetTicks();
                    while (SDL_PollEvent(&event)) {
                        running = (event.type != SDL_QUIT);
                    }
                    UpdateFrame();
                    u32 end = SDL_GetTicks() - start;
                    if (end < TPF) {
                        SDL_Delay(TPF-end);
                    }
                }
            }
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
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
