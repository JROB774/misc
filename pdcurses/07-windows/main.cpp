/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>

WINDOW* CreateWin  (int _h, int _w, int _sy, int _sx);
void    DestroyWin (WINDOW* _window);

int main (int _argc, char* _argv[])
{
    initscr();

    cbreak();
    keypad(stdscr, true);
    noecho(); // Otherwise enter will erase lines!

    int win_h  = 3;
    int win_w  = 10;
    int win_sy = (LINES - win_h) / 2;
    int win_sx = (COLS - win_w) / 2;

    printw("Press F1 to exit the program!");
    refresh();

    WINDOW* window = CreateWin(win_h, win_w, win_sy, win_sx);

    // This is extremely inefficient! See the following example listed
    // for a more efficient method of creating the same functionality.
    int input = 0;
    while ((input = getch()) != KEY_F(1)) {
        switch (input) {
            case (KEY_LEFT): {
                if (win_sx != 0) {
                    DestroyWin(window);
                    window = CreateWin(win_h, win_w, win_sy, --win_sx);
                }
            } break;
            case (KEY_RIGHT): {
                if ((win_sx + win_w) != COLS) {
                    DestroyWin(window);
                    window = CreateWin(win_h, win_w, win_sy, ++win_sx);
                }
            } break;
            case (KEY_UP): {
                if (win_sy != 0) {
                    DestroyWin(window);
                    window = CreateWin(win_h, win_w, --win_sy, win_sx);
                }
            } break;
            case (KEY_DOWN): {
                if ((win_sy + win_h) != LINES) {
                    DestroyWin(window);
                    window = CreateWin(win_h, win_w, ++win_sy, win_sx);
                }
            } break;
        }
    }

    endwin();
    return 0;
}

WINDOW* CreateWin (int _h, int _w, int _sy, int _sx)
{
    WINDOW* window = newwin(_h, _w, _sy, _sx);

    // 0,0 == use defualt chars for horz and vert lines.
    box(window, 0, 0);
    wrefresh(window);

    return window;
}

void DestroyWin (WINDOW* _window)
{
    // To clear the surrounding window border.
    // Turn this off to leave a trail of windows!
    wborder(_window, ' ',' ',' ',' ',' ',' ',' ',' ');
    wrefresh(_window);

    delwin(_window);
}
