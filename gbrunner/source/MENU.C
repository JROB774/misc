#define MENU_PRESS_START_BLINK_INTERVAL 0x1E // 30

typedef struct _Menu_
{
    U8 show_text; // Whether the PRESS START TEXT should be displayed or not.
    U8 pressed;   // Whether the start button is pressed or not.
    U8 timer;     // Timer that ticks up every game loop.

} Menu;

global const U8 MENU_PRESS_START_ON[] =
{
    0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D
};
global const U8 MENU_PRESS_START_OFF[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

global Menu menu;

internal void transition_from_menu_to_game ()
{
    fade_to_white();
    set_game_state(GAME_STATE_GAME);
    fade_from_white();
}

internal void init_menu ()
{
    menu.show_text = FALSE;
    menu.pressed = TRUE;
    menu.timer = 0;
}

internal void update_menu ()
{
    HIDE_WIN;
    HIDE_SPRITES;
    SHOW_BKG;

    // Every 1/2 second toggle the PRESS START text on and off.
    menu.timer++;
    if (menu.timer == MENU_PRESS_START_BLINK_INTERVAL)
    {
        menu.show_text = !menu.show_text;
        menu.timer = 0;

        if (menu.show_text) { set_bkg_tiles(0x05,0x0D, 10,1, MENU_PRESS_START_ON ); }
        else                { set_bkg_tiles(0x05,0x0D, 10,1, MENU_PRESS_START_OFF); }
    }

    // We do this so that if the splash screen was skipped by pressing START
    // then the button has to be pressed again on the menu to begin the game.
    if (!(joypad() & J_START))
    {
        menu.pressed = FALSE;
    }

    if ((joypad() & J_START) && !menu.pressed)
    {
        transition_from_menu_to_game();
    }
}
