#define SCORE_WIDTH  SCREEN_WIDTH
#define SCORE_HEIGHT 1

#define MAXIMUM_SCORE 999999

global const U8 EMPTY_SCORE[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

global U32 current_score;

internal void hide_score_interrupt ()
{
    HIDE_WIN;
}

internal void update_score ()
{
    U32 temp_score;
    U8 num_drawn;

    // Cap the score at its maximum value.
    if (current_score < MAXIMUM_SCORE) { current_score++; }

    temp_score = current_score;
    num_drawn = 0;

    while (temp_score)
    {
        U8 digit = temp_score % 10 + 1;
        set_win_tiles(12-num_drawn,0, 1,1, &digit);
        num_drawn++;
        temp_score /= 10;
    }
}

internal void init_score ()
{
    set_win_tiles(0,0, SCORE_WIDTH,SCORE_HEIGHT, EMPTY_SCORE);

    // Sets up an interrupt handler to disable showing the window layer after a certain point.
    // This allows us to have our heads-up display at the top of the screen without issues.

    STAT_REG |= 0x40; // Enable LYC=LY interrupt.
    LYC_REG   = 0x07; // The scanline on which to trigger.
    disable_interrupts();
    add_LCD(hide_score_interrupt);
    enable_interrupts();
    set_interrupts(LCD_IFLAG|VBL_IFLAG);

    current_score = 0;
}
