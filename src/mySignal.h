#ifndef MYSIGNAL_H
#define MYSIGNAL_H

#include <signal.h>

#include "library.c"

void usersignalHandler(int);
void timerHandler(int);

#endif