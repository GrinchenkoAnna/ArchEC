#include "myTerm.h"

int mt_clrscr(void)
{
    printf("\E[H\E[J");
    return 0;
}

int mt_gotoXY(int x, int y)
{
    printf("\E[%d;%dH", x, y);
    return 0;
}

int mt_getscreensize(int* rows, int* cols)
{
    struct winsize buffer;
    if (ioctl(0, TIOCGWINSZ, &buffer) == -1) { return -1; }
    *rows = buffer.ws_row;
    *cols = buffer.ws_col;        
    return 0;
}

int mt_setfgcolor(dye color)
{
    printf("\E[%dm", color);
    return 0;
}

int mt_setbgcolor(dye color)
{
    printf("\E[%dm", color + 10);
    return 0;
}

int mt_resetcolor()
{
    printf("\E[0m");
    return 0;
}