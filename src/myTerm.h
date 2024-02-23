#ifndef MYTERM_H
#define MYTERM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef enum colors 
{
  BLACK = 30,
  RED,        //31
  GREEN,      //32
  YELLOW,      //33
  BLUE,       //34
  PURPLE,     //35
  CYAN,       //36
  LIGHT_GRAY, //37
  WHITE = 97
} dye;

int mt_readfromterm(char*, int);
int mt_printtoterm(char*);

int mt_clrscr(void);
int mt_gotoXY(int, int);
int mt_getscreensize(int*, int*);
int mt_setfgcolor(dye);
int mt_setbgcolor(dye);
int mt_resetcolor();

#endif
