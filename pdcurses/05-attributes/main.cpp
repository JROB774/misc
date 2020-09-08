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

    int rows = 0, cols = 0;
    getmaxyx(stdscr, rows, cols);

    // attrset() - Resets all the window's attributes.
    // attron()  - Turns on the specific attributes.

    // Prints the whole file to the terminal -- in bold.
    attron(A_BOLD);
    for (int i=1; i<_argc; ++i) {
        FILE* file = fopen(_argv[i], "r");
        if (!file) { continue; }

        int ypos = 0, xpos = 0;
        int current;
        while ((current = fgetc(file)) != EOF) {
            // Print the character in bold.
            mvaddch(ypos, xpos, current);
            refresh();

            // If the line is too large to fit on screen add basic scroll.
            if (ypos == (rows - 1)) {
                printw("[PRESS ANY KEY]");
                getch();
                clear();
                move(0, 0);
            }
            getyx(stdscr, ypos, xpos);
        }

        fclose(file);
        file = NULL;
    }
    attroff(A_BOLD);

    getch();
    endwin();

    return 0;
}
