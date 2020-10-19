#define PLAYER_START_X_POS   0x10 //  16
#define PLAYER_START_Y_POS   0x68 // 104

#define PLAYER_JUMP_FORCE    0x32 //  50
#define PLAYER_JUMP_FRAC     0x0A //  10
#define PLAYER_GRAVITY       0x05 //   5

#define PLAYER_STATE_RUN_0   0x00
#define PLAYER_STATE_RUN_1   0x01
#define PLAYER_STATE_JUMP    0x02
#define PLAYER_STATE_SLIDE   0x03
#define PLAYER_STATE_DEAD_0  0x04
#define PLAYER_STATE_DEAD_1  0x05

#define PLAYER_BUTTON_JUMP   (J_B|J_UP  )
#define PLAYER_BUTTON_SLIDE  (J_A|J_DOWN)

typedef struct _Player_State_
{
    U8 sprite_offset;  // Offset into VRAM for this state's animation.
    U8 ticks;          // Number of ticks before changing to next_state.
    U8 next_state;     // Next state to jump to after this one.

} Player_State;

typedef struct _Player_
{
    U8 sprite_slot[4]; // OAM sprite slots occupied by the player object.
    U8 x, y;           // Current (X,Y) position.
    U8 state;          // Current animation/state.
    U8 jumped;         // Boolean set when jump is pressed and reset on release.
    S8 force;          // Force applied on the player whilst they're jumping.
    U8 timer;          // Timer that ticks up every game loop.
    U8 dead_pressed;   // Whether any button is pressed when entering dead.

} Player;

global const Player_State PLAYER_STATE_TABLE[] =
{
    // sprite_offset  ticks  next_state
    { 0x00,           10,    PLAYER_STATE_RUN_1  }, // PLAYER_STATE_RUN_0
    { 0x04,           10,    PLAYER_STATE_RUN_0  }, // PLAYER_STATE_RUN_1
    { 0x04,           0,     PLAYER_STATE_JUMP   }, // PLAYER_STATE_JUMP
    { 0x08,           0,     PLAYER_STATE_SLIDE  }, // PLAYER_STATE_SLIDE
    { 0x0C,           10,    PLAYER_STATE_DEAD_1 }, // PLAYER_STATE_DEAD_0
    { 0x10,           10,    PLAYER_STATE_DEAD_0 }  // PLAYER_STATE_DEAD_1
};

global Player player;

// Predeclare this function as the player needs to know about it.
internal void reset_game ();

internal void set_player_state (U8 state)
{
    if (player.state == state) { return; }

    player.state = state;
    player.timer = 0; // Reset this for animations, etc.

    set_sprite_tile(player.sprite_slot[0], PLAYER_STATE_TABLE[player.state].sprite_offset+0);
    set_sprite_tile(player.sprite_slot[1], PLAYER_STATE_TABLE[player.state].sprite_offset+1);
    set_sprite_tile(player.sprite_slot[2], PLAYER_STATE_TABLE[player.state].sprite_offset+2);
    set_sprite_tile(player.sprite_slot[3], PLAYER_STATE_TABLE[player.state].sprite_offset+3);
}

internal void set_player_pos (U8 x, U8 y)
{
    player.x = x;
    player.y = y;

    // Get player meta-sprites to draw down from the top-left corner.
    x += TILE_WIDTH;
    y += TILE_HEIGHT * 2;

    move_sprite(player.sprite_slot[0], x,            y            );
    move_sprite(player.sprite_slot[1], x+TILE_WIDTH, y            );
    move_sprite(player.sprite_slot[2], x,            y+TILE_HEIGHT);
    move_sprite(player.sprite_slot[3], x+TILE_WIDTH, y+TILE_HEIGHT);
}

internal void maybe_make_the_player_jump ()
{
    if ((joypad() & PLAYER_BUTTON_JUMP) && !player.jumped)
    {
        set_player_state(PLAYER_STATE_JUMP);
        player.jumped = TRUE;
        player.force = PLAYER_JUMP_FORCE;
    }

    // Wait until the jump key is released before allowing the player to jump again.
    if (!(joypad() & PLAYER_BUTTON_JUMP))
    {
        player.jumped = FALSE;
    }
}

internal U8 is_player_dead ()
{
    return (player.state == PLAYER_STATE_DEAD_0 || player.state == PLAYER_STATE_DEAD_1);
}

internal void init_player ()
{
    // The player sprite always occupies OAM slots 0-3.
    player.sprite_slot[0] = 0;
    player.sprite_slot[1] = 1;
    player.sprite_slot[2] = 2;
    player.sprite_slot[3] = 3;

    set_player_pos(PLAYER_START_X_POS, PLAYER_START_Y_POS);

    // Set the player's state to an invalid value to ensure that
    // the call to set_player_state carries out the needed logic.
    player.state = 0xFF;

    set_player_state(PLAYER_STATE_RUN_0);

    player.dead_pressed = FALSE;
}

internal void update_player ()
{
    // Update the player's states and animations.
    player.timer++;
    if (player.timer == PLAYER_STATE_TABLE[player.state].ticks)
    {
        set_player_state(PLAYER_STATE_TABLE[player.state].next_state);
    }

    // Logic for the various different player states.
    switch (player.state)
    {
        case (PLAYER_STATE_RUN_0):
        case (PLAYER_STATE_RUN_1):
        {
            if ((joypad() & PLAYER_BUTTON_SLIDE))
            {
                set_player_state(PLAYER_STATE_SLIDE);
            }

            maybe_make_the_player_jump();
        } break;
        case (PLAYER_STATE_JUMP):
        {
            player.y -= player.force / PLAYER_JUMP_FRAC;
            player.force -= PLAYER_GRAVITY;

            // Once the player has hit the ground set them back to running.
            if (player.y > PLAYER_START_Y_POS)
            {
                player.y = PLAYER_START_Y_POS;
                set_player_state(PLAYER_STATE_RUN_0);
            }
        } break;
        case (PLAYER_STATE_SLIDE):
        {
            // If the slide button is no longer being pressed return to running.
            if (!(joypad() & PLAYER_BUTTON_SLIDE))
            {
                set_player_state(PLAYER_STATE_RUN_0);
            }

            maybe_make_the_player_jump();
        } break;
        case (PLAYER_STATE_DEAD_0):
        case (PLAYER_STATE_DEAD_1):
        {
            // If any key is pressed then we reset the game.
            if (!joypad())
            {
                player.dead_pressed = FALSE;
            }
            if (joypad() && !player.dead_pressed)
            {
                fade_to_white();
                reset_game();
                fade_from_white();
            }
        } break;
    }

    // Update the player's position.
    set_player_pos(player.x, player.y);
}
