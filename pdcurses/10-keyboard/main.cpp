/******************************************************************************/
/*                                                                            */
/*    TUTORIAL USED: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/     */
/*                                                                            */
/******************************************************************************/

#include <pdcurses/curses.h>

static constexpr int HEIGHT = 10;
static constexpr int WIDTH  = 30;

static constexpr const char* CHOICES[] =
{ "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Exit", };

static constexpr int NUM_CHOICES = sizeof(CHOICES) / sizeof(char*);

static int start_y = 0;
static int start_x = 0;

void PrintMenu (WINDOW* _menu, int _highlight);

int main (int _argc, char* _argv[])
{
    initscr();

    clear();
    noecho();
    cbreak();

    start_y = (LINES - HEIGHT) / 2;
    start_x = (COLS - WIDTH) / 2;

    WINDOW* menu = newwin(HEIGHT, WIDTH, start_y, start_x);
    keypad(menu, true);

    mvprintw(0, 0, "Use arrow keys to go up and down, enter to select.");
    refresh();

    int highlight = 0;
    int choice = 0;
    while (true) {
        PrintMenu(menu, highlight);
        int input = wgetch(menu);
        switch (input) {
            case (KEY_UP):   { highlight = (highlight == 0) ? NUM_CHOICES-1 : --highlight; } break;
            case (KEY_DOWN): { highlight = (highlight == NUM_CHOICES-1) ? 0 : ++highlight; } break;

            case (10): { // Enter key.
                choice = highlight;
            } break;
        }

        // If they chose exit, then exit.
        if (choice == NUM_CHOICES-1) { break; }
        refresh();
    }

    endwin();
    return 0;
}

void PrintMenu (WINDOW* _menu, int _highlight)
{
    int y = 2;
    int x = 2;

    box(_menu, 0, 0);
    for (int i=0; i<NUM_CHOICES; ++i) {
        if (_highlight != i) {
            mvwprintw(_menu, y, x, "%s", CHOICES[i]);
        } else {
            wattron(_menu, A_REVERSE);
            mvwprintw(_menu, y, x, "%s", CHOICES[i]);
            wattroff(_menu, A_REVERSE);
        }
        ++y;
    }

    wrefresh(_menu);
}
