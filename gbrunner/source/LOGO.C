#define LOGO_DURATION 120

typedef struct _Logo_
{
    U8 timer; // Timer that ticks up every game loop.

} Logo;

global Logo logo;

internal void transition_from_logo_to_menu ()
{
    fade_to_white();
    set_game_state(GAME_STATE_MENU);
    fade_from_white();
}

internal void init_logo ()
{
    logo.timer = 0;
}

internal void update_logo ()
{
    HIDE_WIN;
    HIDE_SPRITES;
    SHOW_BKG;

    // If the user presses any button then skip the splash screen.
    if (joypad())
    {
        transition_from_logo_to_menu();
    }

    // Transition to the menu state after 2 seconds.
    logo.timer++;
    if (logo.timer >= LOGO_DURATION)
    {
        transition_from_logo_to_menu();
    }
}
