#include "myBigChars.h"

int bc_printA(char *str)
{
    printf("\E(0");
    printf("%s", str);
    printf("\E(B");

    return 0;
}

