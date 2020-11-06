/*******************************************************************************
 * A simple emulator implementation for the CHIP8 computer system.
 * Available Under MIT License (See EOF)
 * Released 20-07-2020
 * Version 1.0.1
 * Authored by Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

// This is only needed in console/debug based builds of the application.
// If it is enabled in a release build then the compiler will complain.
#if defined(BUILD_DEBUG)
#define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>

/* -------------------------------------------------------------------------- */

// GENERAL UTILITIES

#define GLOBAL static
#define STDDEF static
#define INLDEF static inline

typedef uint8_t   U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t    S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;

#if defined(BUILD_DEBUG)
#define LOG_ERROR(...) do { fprintf(stderr, __VA_ARGS__); return 0; } while (0,0)
#else
#define LOG_ERROR(...) return 0;
#endif

/* -------------------------------------------------------------------------- */

// CHIP8 ARCHITECTURE

GLOBAL constexpr int CHIP8_DISPLAY_WIDTH  = 0x040;
GLOBAL constexpr int CHIP8_DISPLAY_HEIGHT = 0x020;
GLOBAL constexpr U16 CHIP8_FONTSET_START  = 0x050; // Starting address for the built-in font.
GLOBAL constexpr U16 CHIP8_PROGRAM_START  = 0x200; // Starting address of most CHIP8 programs.

// CHIP8 defines its font using bits to determine which pixels are active.
// Each of the 8-bit hex values represents a row of pixels per character.
GLOBAL constexpr U8 CHIP8_FONTSET[0x50]
{
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

GLOBAL   U8 MEMORY [0x1000]; // 4kb of RAM (512b is reserved).
GLOBAL   U8 DISPLAY[0x0800]; // Monochrome display.
GLOBAL  U16 STACK  [0x0010]; // Call stack for subroutines.
GLOBAL   U8 KEYS   [0x0010]; // Current state of the keys.
GLOBAL   U8 V      [0x0010]; // General-purpose register (VF reserved).
GLOBAL  U16 I;               // Special 16-bit register.
GLOBAL  U16 PC;              // Program counter.
GLOBAL   U8 SP;              // Stack pointer.
GLOBAL   U8 DELAYTIMER;      // Ticks down when not zero.
GLOBAL   U8 SOUNDTIMER;      // Ticks down when not zero (plays beep).
GLOBAL bool WAITING;         // Waiting for key flag.
GLOBAL bool REFRESH;         // Refresh/draw the screen flag.
GLOBAL   U8 WAITKEY = 0xFF;  // Stores key ID pressed when waiting.

#define V0 V[0x0]
#define VF V[0xF]

#define UNKNOWN_OP LOG_ERROR("Unknown OP (%4X)", OP);

// Instructions: https://en.wikipedia.org/wiki/CHIP-8#Opcode_table

#define DO_OP_call   // Usually unimplemented on emulators.
#define DO_OP_cls    memset(&DISPLAY[0], 0, 0x800); REFRESH = true; PC += 2;
#define DO_OP_rts    PC = STACK[--SP] + 2;
#define DO_OP_jmp    PC = NNN;
#define DO_OP_jsr    STACK[SP++] = PC; PC = NNN;
#define DO_OP_skeq1  PC += ((V[X] ==   KK) ? 4 : 2);
#define DO_OP_skne1  PC += ((V[X] !=   KK) ? 4 : 2);
#define DO_OP_skeq2  PC += ((V[X] == V[Y]) ? 4 : 2);
#define DO_OP_mov1   V[X]  =   KK; PC += 2;
#define DO_OP_add1   V[X] +=   KK; PC += 2;
#define DO_OP_mov2   V[X]  = V[Y]; PC += 2;
#define DO_OP_or     V[X] |= V[Y]; PC += 2;
#define DO_OP_and    V[X] &= V[Y]; PC += 2;
#define DO_OP_xor    V[X] ^= V[Y]; PC += 2;
#define DO_OP_add2   VF = (V[Y] > (0xFF - V[X])); V[X] = (((U16)(V[X]) + (U16)(V[Y])) & 0xFF); PC += 2;
#define DO_OP_sub    VF = (V[X] > V[Y]); V[X]  -= V[Y];        PC += 2;
#define DO_OP_shr    VF = (V[X] & 0x01); V[X] >>= 1;           PC += 2;
#define DO_OP_rsb    VF = (V[Y] > V[X]); V[X]   = V[Y] - V[X]; PC += 2;
#define DO_OP_shl    VF = (V[X] >>   7); V[X] <<= 1;           PC += 2;
#define DO_OP_skne2  PC += ((V[X] != V[Y]) ? 4 : 2);
#define DO_OP_mvi    I  = NNN; PC += 2;
#define DO_OP_jmi    PC = NNN + V0;
#define DO_OP_rand   V[X] = (rand() % 256) & KK; PC += 2;
#define DO_OP_skpr   PC += ((KEYS[V[X]]) ? 4 : 2);
#define DO_OP_skup   PC += ((KEYS[V[X]]) ? 2 : 4);
#define DO_OP_gdelay V[X] = DELAYTIMER; PC += 2;
#define DO_OP_key    if (WAITKEY > 0xF) { WAITING = true; } else { V[X] = WAITKEY; WAITKEY = 0xFF; WAITING = false; PC += 2; }
#define DO_OP_sdelay DELAYTIMER = V[X]; PC += 2;
#define DO_OP_ssound SOUNDTIMER = V[X]; PC += 2;
#define DO_OP_adi    I += (U16)(V[X]);  PC += 2;
#define DO_OP_font   I  = (CHIP8_FONTSET_START + ((U16)(V[X])*5)); PC += 2;
#define DO_OP_bcd    MEMORY[I] = ((V[X]/100)%10); MEMORY[I+1] = ((V[X]/10)%10); MEMORY[I+2] = ((V[X]/1)%10); PC += 2;
#define DO_OP_str    for (U8 i=0; i<=X; ++i) { MEMORY[I+i] = V[i]; } PC += 2;
#define DO_OP_ldr    for (U8 i=0; i<=X; ++i) { V[i] = MEMORY[I+i]; } PC += 2;
#define DO_OP_sprite                                                       \
VF = 0;                                                                    \
for (U8 iy=0; iy<(OP & 0x000F); ++iy) {                                    \
    U8 line = MEMORY[I+iy];                                                \
    for (U8 ix=0; ix<8; ++ix) {                                            \
        if ((line & (0x80 >> ix))) {                                       \
            U16 index = ((V[Y] + iy) * CHIP8_DISPLAY_WIDTH + (V[X] + ix)); \
            if (DISPLAY[index] == 1) VF = 1;                               \
            DISPLAY[index] ^= 1;                                           \
        }                                                                  \
    }                                                                      \
}                                                                          \
REFRESH = true;                                                            \
PC += 2;

/* -------------------------------------------------------------------------- */

// EMULATOR FACILITIES

// Maps CHIP8 keys [0x0 to 0xF] to corresponding SDL keycodes.
GLOBAL constexpr SDL_Keycode KEY_MAP[0x10]
{
SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a,
SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

GLOBAL SDL_Window*   window;
GLOBAL SDL_Renderer* renderer;
GLOBAL SDL_Event     event;

/* -------------------------------------------------------------------------- */

int main (int argc, char** argv)
{
    U32 flags = SDL_INIT_VIDEO|SDL_INIT_AUDIO;
    if (SDL_Init(flags) < 0) LOG_ERROR("Failed to initialize SDL! (%s)", SDL_GetError());

    // If no ROM is provided let the user know through a message prompt.
    if (argc < 2) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "CHIP8", "No ROM provided!", NULL);
        return 0;
    }

    // We scale up the window view just so the screen is not tiny.
    constexpr int DRAW_SCALE = 5;

    window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
      CHIP8_DISPLAY_WIDTH*DRAW_SCALE,CHIP8_DISPLAY_HEIGHT*DRAW_SCALE, SDL_WINDOW_SHOWN);
    if (!window) LOG_ERROR("Failed to create window! (%s)", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) LOG_ERROR("Failed to create renderer! (%s)", SDL_GetError());
    SDL_RenderSetScale(renderer, DRAW_SCALE, DRAW_SCALE);

    // Fill the CHIP8's memory with the FONTSET and ROM.
    memcpy(&MEMORY[CHIP8_FONTSET_START], CHIP8_FONTSET, sizeof(CHIP8_FONTSET));
    FILE* rom = fopen(argv[1], "rb");
    if (!rom) LOG_ERROR("Failed to load ROM!");
    fseek(rom, 0L, SEEK_END);
    size_t size = ftell(rom);
    rewind(rom);
    if (size > 0x1000-CHIP8_PROGRAM_START) LOG_ERROR("ROM file too large!");
    fread(&MEMORY[CHIP8_PROGRAM_START], size, 1, rom);
    fclose(rom);
    PC = CHIP8_PROGRAM_START;

    bool running = true;
    while (running) {
        U16 OP  = ((U16)(MEMORY[PC])    << 8) | MEMORY[PC+1];
        U8  X   = (((OP & 0x0F00)       >> 8));
        U8  Y   = (((OP & 0x00F0) << 4) >> 8);
        U16 NNN =   (OP & 0x0FFF);
        U8  KK  =   (OP & 0x00FF);

        switch (OP & 0xF000) {
        case (0x0000): {
        switch (OP & 0x0FFF) {
        case (0x00E0): { DO_OP_cls   ; } break; // 00E0 : Clear the display.
        case (0x00EE): { DO_OP_rts   ; } break; // 00EE : Return from a subroutine.
        default:       { UNKNOWN_OP  ; } break;
        }} break;
        case (0x1000): { DO_OP_jmp   ; } break; // 1nnn : Jump to location nnn.
        case (0x2000): { DO_OP_jsr   ; } break; // 2nnn : Call subroutine at nnn.
        case (0x3000): { DO_OP_skeq1 ; } break; // 3xkk : Skip next instruction if Vx == kk.
        case (0x4000): { DO_OP_skne1 ; } break; // 4xkk : Skip next instruction if Vx != kk.
        case (0x5000): { DO_OP_skeq2 ; } break; // 5xy0 : Skip next instruction if Vx == Vy.
        case (0x6000): { DO_OP_mov1  ; } break; // 6xkk : Set Vx to kk.
        case (0x7000): { DO_OP_add1  ; } break; // 7xkk : Set Vx to Vx + kk.
        case (0x8000): {
        switch (OP & 0x000F) {
        case (0x0000): { DO_OP_mov2  ; } break; // 8xy0 : Set Vx to Vy.
        case (0x0001): { DO_OP_or    ; } break; // 8xy1 : Set Vx to Vx | Vy.
        case (0x0002): { DO_OP_and   ; } break; // 8xy2 : Set Vx to Vx & Vy.
        case (0x0003): { DO_OP_xor   ; } break; // 8xy3 : Set Vx to Vx ^ Vy.
        case (0x0004): { DO_OP_add2  ; } break; // 8xy4 : Set Vx to Vx + Vy, set VF to 1 if carry else 0.
        case (0x0005): { DO_OP_sub   ; } break; // 8xy5 : Set Vx to Vx - Vy, set VF to 1 if not borrow else 0.
        case (0x0006): { DO_OP_shr   ; } break; // 8xy6 : Set Vx to Vx / 02, set VF to 1 if least-significant bit of Vx is 1 else 0.
        case (0x0007): { DO_OP_rsb   ; } break; // 8xy7 : Set Vx to Vy - Vx, set VF to 1 if not borrow else 0.
        case (0x000E): { DO_OP_shl   ; } break; // 8xyE : Set Vx to Vx * 02, set VF to 1 is most-significant big of Vx is 1 else 0.
        default:       { UNKNOWN_OP  ; } break;
        }} break;
        case (0x9000): { DO_OP_skne2 ; } break; // 9xy0 : Skip next instruction if Vx != Vy.
        case (0xA000): { DO_OP_mvi   ; } break; // Annn : Set I to nnn.
        case (0xB000): { DO_OP_jmi   ; } break; // Bnnn : Jump to location nnn + V0.
        case (0xC000): { DO_OP_rand  ; } break; // Cnnn : Set Vx to a random byte & kk.
        case (0xD000): { DO_OP_sprite; } break; // Dxyn : Display n-byte sprite starting at memory location I at (Vx,Vy), set VF to collision.
        case (0xE000): {
        switch (OP & 0x00FF) {
        case (0x009E): { DO_OP_skpr  ; } break; // Ex9E : Skip the next instruction if the key with the value of Vx is pressed.
        case (0x00A1): { DO_OP_skup  ; } break; // ExA1 : Skip the next instruction if the key with the value of Vx is up.
        default:       { UNKNOWN_OP  ; } break;
        }} break;
        case (0xF000): {
        switch (OP & 0x00FF) {
        case (0x0007): { DO_OP_gdelay; } break; // Fx07 : Set Vx to the DELAYTIMER value.
        case (0x000A): { DO_OP_key   ; } break; // Fx0A : Wait for a key press, store the value of that key in Vx.
        case (0x0015): { DO_OP_sdelay; } break; // Fx15 : Set DELAYTIMER to Vx.
        case (0x0018): { DO_OP_ssound; } break; // Fx18 : Set SOUNDTIMER to Vx.
        case (0x001E): { DO_OP_adi   ; } break; // Fx1E : Set I to I + Vx.
        case (0x0029): { DO_OP_font  ; } break; // Fx29 : Set I to the location of sprite for digit Vx.
        case (0x0033): { DO_OP_bcd   ; } break; // Fx33 : Store binary-coded decimal representation of Vx in memory locations: I+0 I+1 I+2.
        case (0x0055): { DO_OP_str   ; } break; // Fx55 : Store registers V0 to Vx in memory starting at location I.
        case (0x0065): { DO_OP_ldr   ; } break; // Fx65 : Read registers V0 to Vx from memory starting at location I.
        default:       { UNKNOWN_OP  ; } break;
        }} break;
        default:       { UNKNOWN_OP  ; } break;
        }

        if (DELAYTIMER) --DELAYTIMER;
        if (SOUNDTIMER) {
            --SOUNDTIMER;
            // @Improve: Play square wave...
        }

        if (REFRESH) {
            SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0xFF);
            for (U16 iy=0; iy<CHIP8_DISPLAY_HEIGHT; ++iy) {
                for (U16 ix=0; ix<CHIP8_DISPLAY_WIDTH; ++ix) {
                    if (DISPLAY[iy * CHIP8_DISPLAY_WIDTH + ix] != 0) {
                        SDL_RenderDrawPoint(renderer, ix,iy);
                    }
                }
            }
            SDL_RenderPresent(renderer);
            REFRESH = false;
        }

        SDL_Delay(1);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case (SDL_KEYDOWN): {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                } // Fall-through.
                case (SDL_KEYUP): {
                    for (U8 i=0x00; i<0x10; ++i) {
                        if (KEY_MAP[i] == event.key.keysym.sym) {
                            KEYS[i] = (event.type == SDL_KEYDOWN);
                            break;
                        }
                    }
                } break;
                case (SDL_QUIT): {
                    running = false;
                } break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * REVISION HISTORY:
 *
 * v1.0.1  [20-07-2020]  Message box is displayed if no ROM is provided.
 * v1.0.0  [27-11-2019]  Initial release.
 *
*******************************************************************************/

/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2019 Joshua Robertson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
*******************************************************************************/
