/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>

#include <stdlib.h>
#include <stdio.h>

int main (int _argc, char* _argv[])
{
    initscr();

    printw("Hello, PDCurses!\nPress any key to continue...");
    refresh();
    getch();

    def_prog_mode(); // Save the state and leave.
    endwin();

    system("dir");
    printf("Press any key to continue...");
    getchar();

    reset_prog_mode();

    printw("We're back!\nPress any key to exit...");
    refresh();

    getch();
    endwin();

    return 0;
}
