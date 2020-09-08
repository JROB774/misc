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

    printw("Upper Left Corner           "); addch(ACS_ULCORNER); printw("\n");
    printw("Lower Left Corner           "); addch(ACS_LLCORNER); printw("\n");
    printw("Lower Right Corner          "); addch(ACS_LRCORNER); printw("\n");
    printw("Tee Pointing Right          "); addch(ACS_LTEE);     printw("\n");
    printw("Tee Pointing Left           "); addch(ACS_RTEE);     printw("\n");
    printw("Tee Pointing Up             "); addch(ACS_BTEE);     printw("\n");
    printw("Tee Pointing Down           "); addch(ACS_TTEE);     printw("\n");
    printw("Horizontal Line             "); addch(ACS_HLINE);    printw("\n");
    printw("Vertical Line               "); addch(ACS_VLINE);    printw("\n");
    printw("Large Plus or Cross Over    "); addch(ACS_PLUS);     printw("\n");
    printw("Scan Line 1                 "); addch(ACS_S1);       printw("\n");
    printw("Scan Line 3                 "); addch(ACS_S3);       printw("\n");
    printw("Scan Line 7                 "); addch(ACS_S7);       printw("\n");
    printw("Scan Line 9                 "); addch(ACS_S9);       printw("\n");
    printw("Diamond                     "); addch(ACS_DIAMOND);  printw("\n");
    printw("Checker Board (Stipple)     "); addch(ACS_CKBOARD);  printw("\n");
    printw("Degree Symbol               "); addch(ACS_DEGREE);   printw("\n");
    printw("Plus/Minus Symbol           "); addch(ACS_PLMINUS);  printw("\n");
    printw("Bullet                      "); addch(ACS_BULLET);   printw("\n");
    printw("Arrow Pointing Left         "); addch(ACS_LARROW);   printw("\n");
    printw("Arrow Pointing Right        "); addch(ACS_RARROW);   printw("\n");
    printw("Arrow Pointing Down         "); addch(ACS_DARROW);   printw("\n");
    printw("Arrow Pointing Up           "); addch(ACS_UARROW);   printw("\n");
    printw("Board of Squares            "); addch(ACS_BOARD);    printw("\n");
    printw("Lantern Symbol              "); addch(ACS_LANTERN);  printw("\n");
    printw("Solid Square Block          "); addch(ACS_BLOCK);    printw("\n");
    printw("Less/Equal Sign             "); addch(ACS_LEQUAL);   printw("\n");
    printw("Greater/Equal Sign          "); addch(ACS_GEQUAL);   printw("\n");
    printw("Pi                          "); addch(ACS_PI);       printw("\n");
    printw("Not Equal                   "); addch(ACS_NEQUAL);   printw("\n");
    printw("UK pound Sign               "); addch(ACS_STERLING); printw("\n");

    refresh();
    getch();
    endwin();

    return 0;
}
