#include "../src/display.c"

int main()
{
	signal(SIGALRM, timerHandler);
    signal(SIGUSR1, usersignalHandler);

	sc_memorySet(20, 1234);
	int value;
	sc_memoryGet(20, &value);
	printf();
}
