/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>

int main (int _argc, char* _argv[])
{
    initscr();
    raw();                // Line buffering is disabled.
    keypad(stdscr, true); // Extra keys enabled, e.g. F-Keys and Arrows.
    noecho();             // User input is not echoed to the terminal.

    // Calling raw() prevented the need for enter being pressed.
    printw("Type any character!\n");
    int character = getch();

    printw("The pressed key is: ");
    attron(A_BOLD);
    printw("%c", character);
    attroff(A_BOLD);

    // The above printing code could have also been implemented as:
    //
    // printw("The pressed key is: ");
    // printw("%c", character | A_BOLD);

    refresh();
    getch();
    endwin();

    return 0;
}
