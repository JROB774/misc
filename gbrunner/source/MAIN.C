#include <gb/gb.h>

#include "UTILITY.C"

#include "TILELOGO.C"
#include "TILEBKG.C"
#include "TILEMENU.C"
#include "TILESPR.C"

#include "MAPLOGO.C"
#include "MAPMENU.C"
#include "MAPPAUSE.C"
#include "MAPGAME.C"

#include "STATE.C"
#include "FADE.C"
#include "SCORE.C"
#include "PLAYER.C"

#include "LOGO.C"
#include "MENU.C"
#include "GAME.C"

void main ()
{
    set_sprite_data(0,TILESPR_NUM_TILES, TILESPR);

    init_logo();
    init_menu();
    init_game();

    // Set the current palette to all white.
    BGP_REG = 0x00;

    DISPLAY_ON;
    SHOW_BKG;

    set_game_state(GAME_STATE_LOGO);
    fade_from_white();

    while (TRUE)
    {
        switch (game_state)
        {
            case (GAME_STATE_LOGO): update_logo(); break;
            case (GAME_STATE_MENU): update_menu(); break;
            case (GAME_STATE_GAME): update_game(); break;
        }

        wait_vbl_done();
    }
}
