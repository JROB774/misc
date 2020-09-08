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

    // noecho(); // You won't be able to see what you're typing!

    // Get the number of rows and columns in the command-line.
    // This is a macro so there is no need to pass pointers!
    int rows = 0, cols = 0;
    getmaxyx(stdscr, rows, cols);

    const char* MESSAGE = "Enter a string: ";
    mvprintw(rows/2, (cols-strlen(MESSAGE))/2, "%s", MESSAGE);

    char user_input[1024] = {};
    getstr(user_input);

    mvprintw(rows-2, 0, "You entered '%s'.", user_input);

    refresh();
    getch();
    endwin();

    return 0;
}
