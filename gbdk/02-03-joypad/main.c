#include <gb/gb.h>
#include <stdio.h>

#include "sprites.h"
#include "sprites.c"

void main ()
{
    // UINT8 sprite_index = 0;

    set_sprite_data(0,2, SPRITES);
    set_sprite_tile(0,0);
    move_sprite(0, 88,78);

    SHOW_SPRITES;

    while (1)
    {
        if (joypad() & J_LEFT ) scroll_sprite(0, -3, 0);
        if (joypad() & J_RIGHT) scroll_sprite(0,  3, 0);
        if (joypad() & J_UP   ) scroll_sprite(0,  0,-3);
        if (joypad() & J_DOWN ) scroll_sprite(0,  0, 3);

        wait_vbl_done();
    }
}
