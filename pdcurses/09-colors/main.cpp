/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>
#include <string.h>

void PrintInCenter (WINDOW* _window, int _sy, int _sx, int _w, char* _text);

int main (int _argc, char* _argv[])
{
    initscr();

    if (!has_colors()) {
        printw("Terminal has no color support!");
    } else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_RED);

        // We can change the actual RGB (0-1000) values of colors.
        // Use can_change_color() to see if this feature is available.
        init_color(COLOR_WHITE, 1000, 1000, 1000);
        init_color(COLOR_RED,   1000,    0,    0);

        attron(COLOR_PAIR(1));
        PrintInCenter(NULL, LINES/2, 0, COLS, "We're in COLOR!");
        attroff(COLOR_PAIR(1));
    }

    getch();
    endwin();

    return 0;
}

void PrintInCenter (WINDOW* _window, int _sy, int _sx, int _w, char* _text)
{
    if (!_window) { _window = stdscr; }

    int ypos = 0, xpos = 0;
    getyx(_window, ypos, xpos);

    if (_sx != 0) { xpos = _sx; }
    if (_sy != 0) { ypos = _sy; }
    if (_w  == 0) { _w   =  80; }

    int length = strlen(_text);
    float temp = (_w - length) / 2;

    xpos = _sx + (int)temp;
    mvwprintw(_window, ypos, xpos, "%s", _text);
    refresh();
}
