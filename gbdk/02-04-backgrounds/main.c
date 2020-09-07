#include <gb/gb.h>
#include <stdio.h>

#include "backgrounds.h"
#include "backgrounds.c"

#include "sprites.h"
#include "sprites.c"

#include "map.h"
#include "map.c"

void main ()
{
    set_bkg_data(0,7, BACKGROUNDS);
    set_bkg_tiles(0,0, 40,18, MAP);

    SHOW_BKG;
    DISPLAY_ON;

    while (1)
    {
        scroll_bkg(1,0);
        wait_vbl_done();
    }
}
