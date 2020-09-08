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

    // Get the number of rows and columns in the command-line.
    // This is a macro so there is no need to pass pointers!
    int rows = 0, cols = 0;
    getmaxyx(stdscr, rows, cols);

    const char* MESSAGE = "Just a string!";
    mvprintw(rows/2, (cols-strlen(MESSAGE))/2, "%s", MESSAGE);

    mvprintw(rows-2, 0, "This screen has %d rows and %d cols.\n", rows, cols);
    printw("Try resizing your window if possible, and then run again.");

    refresh();
    getch();
    endwin();

    return 0;
}
