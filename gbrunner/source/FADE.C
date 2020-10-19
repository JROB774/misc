// Note: Fading doesn't seem to operate on sprites so we hide them before
//       starting. Look into if there is a way to possibly fade sprites.

#define FADE_STEP_AMOUNT 0x04 // 4
#define FADE_WAIT_PERIOD 0x03 // 3

global U8 fade_step;
global U8 fade_wait;

internal void fade_to_white ()
{
    HIDE_SPRITES;

    for (fade_step=0; fade_step<FADE_STEP_AMOUNT; ++fade_step)
    {
        switch (fade_step)
        {
            case (0): BGP_REG = 0xE4; break;
            case (1): BGP_REG = 0x90; break;
            case (2): BGP_REG = 0x40; break;
            case (3): BGP_REG = 0x00; break;
        }

        for (fade_wait=0; fade_wait<FADE_WAIT_PERIOD; ++fade_wait)
        {
            wait_vbl_done();
        }
    }
}

internal void fade_from_white ()
{
    HIDE_SPRITES;

    for (fade_step=0; fade_step<FADE_STEP_AMOUNT; ++fade_step)
    {
        switch (fade_step)
        {
            case (0): BGP_REG = 0x00; break;
            case (1): BGP_REG = 0x40; break;
            case (2): BGP_REG = 0x90; break;
            case (3): BGP_REG = 0xE4; break;
        }

        for (fade_wait=0; fade_wait<FADE_WAIT_PERIOD; ++fade_wait)
        {
            wait_vbl_done();
        }
    }
}

internal void fade_to_black ()
{
    HIDE_SPRITES;

    for (fade_step=0; fade_step<FADE_STEP_AMOUNT; ++fade_step)
    {
        switch (fade_step)
        {
            case (0): BGP_REG = 0xE4; break;
            case (1): BGP_REG = 0xF9; break;
            case (2): BGP_REG = 0xFE; break;
            case (3): BGP_REG = 0xFF; break;
        }

        for (fade_wait=0; fade_wait<FADE_WAIT_PERIOD; ++fade_wait)
        {
            wait_vbl_done();
        }
    }
}

internal void fade_from_black ()
{
    HIDE_SPRITES;

    for (fade_step=0; fade_step<FADE_STEP_AMOUNT; ++fade_step)
    {
        switch (fade_step)
        {
            case (0): BGP_REG = 0xFF; break;
            case (1): BGP_REG = 0xFE; break;
            case (2): BGP_REG = 0xF9; break;
            case (3): BGP_REG = 0xE4; break;
        }

        for (fade_wait=0; fade_wait<FADE_WAIT_PERIOD; ++fade_wait)
        {
            wait_vbl_done();
        }
    }
}
