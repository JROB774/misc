#include <gb/gb.h>

void main ()
{
    DISPLAY_ON;      // Enable display.
    NR52_REG = 0x7F; // Enable sound.
    NR51_REG = 0x11; // Enable the sound channels.
    NR50_REG = 0x77; // Increase volume to max.

    while (1)
    {
        if (joypad() & J_B)
        {
            // The B button was pressed...
        }

        // Game logic...
    }
}
