#include <gb/gb.h>

#include "alpha.c"
#include "hello.c"
#include "blank.c"

#include "sprite.c"

// The player array will hold the player's position as X ([0]) and Y ([1])
UINT8 player[2];

void main ()
{
    DISPLAY_ON;         // Enable display.
    // NR52_REG = 0x7F; // Enable sound.
    // NR51_REG = 0x11; // Enable the sound channels.
    // NR50_REG = 0x77; // Increase volume to max.

    // Load 47 tiles into background memory.
    set_bkg_data(0, 47, alpha);
    // Load 1 tiles into sprite memory.
    set_sprite_data(0, 1, sprites);

    // Use the 'helloWorld' array to write background tiles starting at 0,6
    // (tile positions) and write for 10 tiles in width, 2 tiles in height.
    set_bkg_tiles(0,6, 10,2, helloWorld);

    // Set the first movable sprite (0) to be the first tile in the sprite memory (0).
    set_sprite_tile(0,0);

    player[0] = 80;
    player[1] = 72;

    while (1)
    {
        if (joypad() & J_B) set_bkg_tiles(0,0, 20,18, blankScreen);

        // Move the player sprite around the screen.
        if (joypad() & J_UP   ) player[1]--;
        if (joypad() & J_DOWN ) player[1]++;
        if (joypad() & J_LEFT ) player[0]--;
        if (joypad() & J_RIGHT) player[0]++;

        // Move the sprite in the first movable sprite list (0)
        // to the position of X (player[0]) and y (player[1]).
        move_sprite(0, player[0], player[1]);

        HIDE_WIN;
        SHOW_SPRITES;
        SHOW_BKG;

        // Wait until VBLANK to avoid corrupting visual memory.
        wait_vbl_done();
    }
}
