#include "mySignal.h"

void usersignalHandler(int signo)
{	
	sc_memoryInit();
    sc_regInit();

    sc_regSet(IGNORING_CLOCK_PULSES, 1);
    instructionCounter = 0;
    accumulator = 0;    
}

void timerHandler(int signo)
{
	int value;
	sc_regGet(IGNORING_CLOCK_PULSES, &value); // проверка (4)
	
	if ((instructionCounter >= 0 && instructionCounter < 99) && (value == 0)) //(4) = 0
	{ instructionCounter++; }
	else if (instructionCounter >= 99) { instructionCounter = 0; }

	if (value == 0) { alarm(1); } 
}