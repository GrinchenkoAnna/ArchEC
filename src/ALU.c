#include "ALU.h"

int ALU(int command, int operand)
{
    switch (command)
    {
    case ADD:
        accumulator += operand; break;

    case SUB:
        accumulator -= operand; break;

    case DIVIDE:
        if(operand != 0)
        {
            sc_regSet(DIVISION_BY_ZERO, 0);
            accumulator /= operand;
        }
        else
        {
            sc_regSet(DIVISION_BY_ZERO, 1);
            return -1;
        }
        break;

    case MUL:
        accumulator *= operand; break;
    }

    return 0;
}
