/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>

int main (int _argc, char* _argv[])
{
    initscr();

    printw("Hello, PDCurses!");
    refresh();

    getch();
    endwin();

    return 0;
}
