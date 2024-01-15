#ifndef MYBIGCHARS_H
#define MYBIGCHARS_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "myTerm.h"

#define BIGCHARSIZE 8

int bigchar_plus[2] = { 134742016, 526398 };
int bigchar_minus[2] = { 0, 62 };
int bigchar_1[2] = { 672663552, 3934216 };
int bigchar_2[2] = { 136583168, 3940368 };
int bigchar_3[2] = { 136583168, 1582084 };
int bigchar_4[2] = { 606348288, 263228 };
int bigchar_5[2] = { 941636608, 1582084 };
int bigchar_6[2] = { 941628416, 1582116 };
int bigchar_7[2] = { 268975104, 1052688 };
int bigchar_8[2] = { 405018624, 1582116 };
int bigchar_9[2] = { 606345216, 3671068 };
int bigchar_0[2] = { 606345216, 1582116 };

int bc_printA(char*);
int bc_box(int, int, int, int);
int bc_setbigcharpos(int*, int, int, int);
int bc_getbigcharpos(int*, int, int, int*);
int bc_bigcharwrite(int, int*, int);
int bc_bigcharread(int, int*, int, int*);

#endif
