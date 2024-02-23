#ifndef SIGNAL_H
#define SIGNAL_H

#include <signal.h>

#include "CU.c"

void set_signals(void);
void usersignalHandler(int);
void timerHandler(int);

#endif

