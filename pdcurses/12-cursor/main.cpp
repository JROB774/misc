/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>
#include <string.h>

int main (int _argc, char* _argv[])
{
    initscr();

    // 0 = Invisible
    // 1 = Visible
    // 2 = Very Visible
    curs_set(0);

    printw("Cursor invisible!\nPress any key to exit.");
    refresh();

    getch();
    endwin();

    return 0;
}
