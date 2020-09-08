/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>

struct WindowBorder
{
    chtype ls, rs, ts, bs;
    chtype tl, tr, bl ,br;
};

struct Window
{
    int sy, sx;
    int  h,  w;

    WindowBorder border;
};

void InitWinParams (Window* _window);
void CreateBox     (Window* _window, bool _flag);

int main (int _argc, char* _argv[])
{
    initscr();

    cbreak();
    keypad(stdscr, true);
    noecho(); // Otherwise enter will erase lines!

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    Window window;
    InitWinParams(&window);

    attron(COLOR_PAIR(1));
    printw("Press F1 to exit the program!");
    refresh();
    attroff(COLOR_PAIR(1));

    CreateBox(&window, true);

    int input = 0;
    while ((input = getch()) != KEY_F(1)) {
        switch (input) {
            case (KEY_LEFT): {
                if (window.sx != 0) {
                    CreateBox(&window, false);
                    --window.sx;
                    CreateBox(&window, true);
                }
            } break;
            case (KEY_RIGHT): {
                if ((window.sx + window.w) != COLS) {
                    CreateBox(&window, false);
                    ++window.sx;
                    CreateBox(&window, true);
                }
            } break;
            case (KEY_UP): {
                if (window.sy != 0) {
                    CreateBox(&window, false);
                    --window.sy;
                    CreateBox(&window, true);
                }
            } break;
            case (KEY_DOWN): {
                if ((window.sy + window.h) != LINES) {
                    CreateBox(&window, false);
                    ++window.sy;
                    CreateBox(&window, true);
                }
            } break;
        }
    }

    endwin();
    return 0;
}

void InitWinParams (Window* _window)
{
    _window->h  = 3;
    _window->w  = 10;
    _window->sy = (LINES - _window->h)/2;
    _window->sx = (COLS - _window->w)/2;

    _window->border.ls = '|';
    _window->border.rs = '|';
    _window->border.ts = '-';
    _window->border.bs = '-';
    _window->border.tl = '+';
    _window->border.tr = '+';
    _window->border.bl = '+';
    _window->border.br = '+';

}

void CreateBox (Window* _window, bool _flag)
{
    int x = _window->sx;
    int y = _window->sy;
    int w = _window->w;
    int h = _window->h;

    if (_flag == true) {
        mvaddch(y,   x,   _window->border.tl);
        mvaddch(y,   x+w, _window->border.tr);
        mvaddch(y+h, x,   _window->border.bl);
        mvaddch(y+h, x+w, _window->border.br);
        mvhline(y,   x+1, _window->border.ts, w-1);
        mvhline(y+h, x+1, _window->border.bs, w-1);
        mvvline(y+1, x,   _window->border.ls, h-1);
        mvvline(y+1, x+w, _window->border.rs, h-1);
    } else {
        for (int iy=y; iy<=(y+h); ++iy) {
            for (int ix=x; ix<=(x+w); ++ix) {
                mvaddch(iy, ix, ' ');
            }
        }
    }

    refresh();
}
