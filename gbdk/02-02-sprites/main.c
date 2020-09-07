#include <gb/gb.h>
#include <stdio.h>

#include "sprites.h"
#include "sprites.c"

void main ()
{
    UINT8 sprite_index = 0;

    set_sprite_data(0,2, SPRITES);
    move_sprite(0, 88,78);

    SHOW_SPRITES;

    while (1)
    {
        sprite_index = (sprite_index == 0) ? 1 : 0;
        set_sprite_tile(0,sprite_index);
        delay(1000);
        scroll_sprite(0, 10,0);
    }
}
