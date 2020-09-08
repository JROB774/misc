/*******************************************************************************
 * Testing SDL's audio API for playing sounds (VOLUME WARNING BEFORE RUNNING).
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include <iostream>
#include <deque>

#include <SDL/SDL.h>

constexpr const char* CAPTION   = "Audio Stream ~ Test";
constexpr int         WIDTH     = 320;
constexpr int         HEIGHT    = 240;
constexpr int         FPS       = 60;
constexpr int         TPF       = 1000 / FPS;

constexpr int         FREQUENCY = 48000;
constexpr uint16_t    FORMAT    = AUDIO_S16SYS;
constexpr uint8_t     CHANNELS  = 2;
constexpr uint16_t    SAMPLES   = 4096;

// secondary buffer we write to, SDL stores the primary buffer that gets
// read internally and we copy data over to that buffer during the callback
std::deque<int16_t> audiobuffer;

int main (int _argc, char* _argv[])
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO); // SDL_INIT_EVENT implicit
    SDL_Surface* window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE|SDL_RESIZABLE);
    SDL_WM_SetCaption(CAPTION, nullptr);

    std::freopen("CON", "w", stdout);
    std::freopen("CON", "w", stderr);
    std::remove("stdout.txt");
    std::remove("stderr.txt");

    SDL_AudioSpec desired = {}, obtained = {};

    desired.freq = FREQUENCY;
    desired.format = FORMAT;
    desired.channels = CHANNELS;
    desired.samples = SAMPLES;
    desired.userdata = nullptr;
    desired.callback = [] (void* _userdata, uint8_t* _stream, int _len)
    {
        int16_t* buffer = reinterpret_cast<int16_t*>(_stream);
        int i = 0;
        while (_len -= 2) {
            if (!audiobuffer.empty()) {
                buffer[i++] = audiobuffer.front();
                audiobuffer.pop_front();
            }
            else { buffer[i++] = 0; }
        }
    };

    SDL_OpenAudio(&desired, &obtained);
    SDL_PauseAudio(0);

    SDL_Event event;
    bool running = true;
    while (running) {
        uint32_t start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case (SDL_QUIT): { running = false; break; }
                case (SDL_KEYUP):
                case (SDL_KEYDOWN): {
                    switch (event.key.keysym.sym) {
                        case (SDLK_ESCAPE): { running = false; break; }
                        default: { break; }
                    }
                    break;
                }
                default: { break; }
            }
        }

        SDL_Flip(window);

        // temp play a square wave
        SDL_LockAudio();
        for (int i = 0; i < (desired.freq / FPS); ++i) {
            if (i < ((desired.freq / FPS) / 2)) {
                audiobuffer.push_back(INT16_MAX);
                audiobuffer.push_back(INT16_MAX);
            }
            else {
                audiobuffer.push_back(INT16_MIN);
                audiobuffer.push_back(INT16_MIN);
            }
        }
        SDL_UnlockAudio();

        uint32_t elapsed = SDL_GetTicks() - start;
        if (elapsed < TPF) { SDL_Delay(TPF - elapsed); }
    }

    SDL_CloseAudio();
    SDL_Quit();

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
