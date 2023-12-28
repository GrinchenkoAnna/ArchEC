#ifndef MYBIGCHARS_H
#define MYBIGCHARS_H

#include <stdio.h>
#include "myTerm.h"

#define BIGCHARSIZE 8

int bc_printA(char*);
int bc_box(int, int, int, int);
int bc_setbigcharpos(int*, int, int, int);

#endif