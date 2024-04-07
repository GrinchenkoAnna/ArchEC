#include "Signal.h"

void set_signals()
{
    signal(SIGALRM, timerHandler);
    signal(SIGUSR1, usersignalHandler);
}

void usersignalHandler(int signo)
{
    sc_memoryInit();
    sc_regInit();

    sc_regSet(IGNORING_CLOCK_PULSES, 1);
    instructionCounter = 0;
    accumulator = 0;

    alarm(0);
}

void timerHandler(int signo)
{
    CU();

    int value;
    sc_regGet(IGNORING_CLOCK_PULSES, &value); // проверка (4)

    if (instructionCounter >= 0 && instructionCounter < 99)
        { instructionCounter++; }
    else if (instructionCounter >= 99) { instructionCounter = 0; }

    if (value == 0) { alarm(1); }

    show_GUI(bg_color, fg_color);
}
