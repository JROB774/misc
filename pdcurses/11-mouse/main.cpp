/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#define NCURSES_MOUSE_VERSION 2

#include <pdcurses/curses.h>
#include <string.h>

int main (int _argc, char* _argv[])
{
    initscr();

    clear();
    noecho();
    cbreak();

    // Setup the mouse event system for use.
    keypad(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION, NULL);

    attron(A_REVERSE);
    int choice = 0;
    while ((choice = getch()) != 'q') {
        MEVENT mouse_event = {};
        getmouse(&mouse_event);

        switch (choice) {
            case (KEY_MOUSE): {
                if (getmouse(&mouse_event) == OK) {
                    if (mouse_event.bstate & BUTTON1_CLICKED) {
                        clear();
                        mvprintw(0, 0, "Press 'Q' to exit the program!");
                        mvprintw(1, 0, "Mouse Clicked: %d %d", mouse_event.y, mouse_event.x);
                        refresh();
                    }
                }
            } break;
        }
    }
    attroff(A_REVERSE);

    endwin();
    return 0;
}
