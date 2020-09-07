#include <gb/gb.h>

#include "alpha.c"
#include "hello.c"
#include "blank.c"

void main ()
{
    DISPLAY_ON;         // Enable display.
    // NR52_REG = 0x7F; // Enable sound.
    // NR51_REG = 0x11; // Enable the sound channels.
    // NR50_REG = 0x77; // Increase volume to max.

    // Load 47 tiles into background memory
    set_bkg_data(0, 47, alpha);

    // Use the 'helloWorld' array to write background tiles starting at 0,6
    // (tile positions) and write for 10 tiles in width, 2 tiles in height.
    set_bkg_tiles(0,6, 10,2, helloWorld);

    while (1)
    {
        if (joypad() & J_B) set_bkg_tiles(0,0, 20,18, blankScreen);

        HIDE_WIN;
        SHOW_SPRITES;
        SHOW_BKG;

        // Wait until VBLANK to avoid corrupting visual memory.
        wait_vbl_done();
    }
}
