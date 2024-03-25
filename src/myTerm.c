#include "myTerm.h"

int mt_readfromterm(char *buffer, int size)
{
    int terminal = open("/dev/tty", O_RDWR);
    if (terminal == -1) { return -1; }

    int symbols_read = read(terminal, buffer, size);
    buffer[symbols_read] = '\0';
    close(terminal);

    return 0;
}

int mt_printtoterm(char *buffer)
{
    int terminal = open("/dev/tty", O_RDWR);
    if (terminal == -1) { return -1; }

    write(terminal, buffer, strlen(buffer));
    close(terminal);

    return 0;
}

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

void mt_enter_alt_charset_mode() { printf("\E(0"); }
void mt_exit_alt_charset_mode() { printf("\E(B"); }
