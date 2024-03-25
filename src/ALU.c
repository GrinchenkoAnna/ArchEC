#include "ALU.h"

int ALU(int command, int operand)
{
    int value;
    sc_memoryGet(operand, &value);

    switch (command)
    {
    case ADD: accumulator += value; break;

    case SUB: accumulator -= value; break;

    case DIVIDE:
        if(value != 0)
        {
            sc_regSet(DIVISION_BY_ZERO, 0);
            accumulator /= value;
        }
        else
        {
            sc_regSet(DIVISION_BY_ZERO, 1);
            return -1;
        }
        break;

    case MUL: accumulator *= value; break;
    }

    return 0;
}
