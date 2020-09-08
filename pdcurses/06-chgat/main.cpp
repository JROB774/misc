/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>
#include <stdlib.h>

int main (int _argc, char* _argv[])
{
    initscr();

    start_color(); // Enable colors.
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    printw("This is my string that chgat() will operate on!");

    // What each of the parameters mean:
    //
    // - Y coordinate to start at.
    // - X coordinate to start at.
    // - Number of characters to update (-1 == to end of line).
    // - The attribute(s) you want to apply to the characters.
    // - Color index pair to apply (or zero for no color).
    // - This should always be set to NULL.
    //
    mvchgat(0, 0, -1, A_BLINK, 1, NULL);

    refresh();
    getch();
    endwin();

    return 0;
}
