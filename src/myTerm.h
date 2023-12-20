#ifndef MYTERM_H
#define MYTERM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

typedef enum colors 
{
    BLACK = 30,
    RED,        //31
    GREEN,      //32
    BROWN,      //33
    BLUE,       //34
    PURPLE,     //35
    CYAN,       //36
    LIGHT_GRAY  //37
} dye;

int mt_clrscr(void);
int mt_gotoXY(int, int);
int mt_getscreensize(int*, int*);
int mt_setfgcolor(dye);
int mt_setbgcolor(dye);

#endif