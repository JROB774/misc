#define GAME_SCROLL_SPEED      0x02 // 2
#define GAME_SCORE_UPDATE_RATE 0x05 // 5

#define GAME_PAUSE_BUTTON (J_START|J_SELECT)

typedef struct _Game_
{
    U8 pause_pressed; // Whether the pause button is pressed or not.
    U8 paused;        // Whether the game is paused or not.
    U8 timer;         // Timer that ticks up every game loop.

} Game;

global Game game;

internal void toggle_pause ()
{
    game.paused = !game.paused;
    game.pause_pressed = TRUE;

    // We add/remove the interrupt so the window layer will display where necessary.
    if (game.paused)
    {
        remove_LCD(hide_score_interrupt);
        set_win_tiles(0,0, MAPPAUSE_WIDTH,MAPPAUSE_HEIGHT, MAPPAUSE);
    }
    else
    {
        add_LCD(hide_score_interrupt);
        set_win_tiles(0,0, SCORE_WIDTH,SCORE_HEIGHT, EMPTY_SCORE);
    }
}

internal void init_game ()
{
    init_player();
    init_score();

    game.pause_pressed = FALSE;
    game.paused = FALSE;

    move_bkg(0,0);
}

internal void reset_game ()
{
    init_game();
}

internal void update_game ()
{
    SHOW_WIN;
    SHOW_SPRITES;
    SHOW_BKG;

    // Cannot pause the game if the player is dead.
    if (!is_player_dead())
    {
        // We do this so that the pause button has to be repressed to trigger it again.
        if (!(joypad() & GAME_PAUSE_BUTTON))
        {
            game.pause_pressed = FALSE;
        }
        // Pause/unpause the game if the start button is pressed.
        if ((joypad() & GAME_PAUSE_BUTTON) && !game.pause_pressed)
        {
            toggle_pause();
        }
    }

    if (!game.paused)
    {
        if (!is_player_dead())
        {
            // Update the score as the player plays the game.
            game.timer++;
            if (game.timer >= GAME_SCORE_UPDATE_RATE)
            {
                game.timer = 0;
                update_score();
            }

            scroll_bkg(GAME_SCROLL_SPEED,0);
        }

        update_player();
    }
}
