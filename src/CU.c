#include "CU.h"

int fREAD(int operand)
{


    return 0;
}

int fWRITE(int operand)
{


    return 0;
}

int fLOAD(int operand)
{
    int value;
    sc_memoryGet(operand, &value);
    accumulator = value;

    return 0;
}

int fSTORE(int operand)
{
    sc_memorySet(operand, accumulator);

    return 0;
}

int fJUMP(int operand)
{
    instructionCounter = operand;
    show_GUI(bg_color, fg_color);
    CU();

    return 0;
}

int fJNEG(int operand)
{
    if (accumulator < 0) { fJUMP(operand); }

    return 0;
}

int fJZ(int operand)
{
    if (accumulator == 0) { fJUMP(operand); }

    return 0;
}

int fHALT()
{
    sc_regSet(IGNORING_CLOCK_PULSES, 1);
    instructionCounter = 0;
    show_GUI(bg_color, fg_color);

    return 0;
}

int CU()
{
    int value, command, operand;
    sc_memoryGet(instructionCounter, &value);
    if (sc_commandDecode(value, &command, &operand) == -1)
    {
        sc_regSet(INVALID_COMMAND, 1);
        sc_regSet(IGNORING_CLOCK_PULSES, 1);
        return -1;
    }

    if ((command >= 30 && command <= 33) || (command >= 51 && command <= 76))
    {
        ALU(command, operand);
    }
    else
    {
        switch (command)
        {
        case 10:
            fREAD(operand); break;

        case 11:
            fWRITE(operand); break;

        case 20:
            fLOAD(operand); break;

        case 21:
            fSTORE(operand); break;

        case 40:
            fJUMP(operand); break;

        case 41:
            fJNEG(operand); break;

        case 42:
            fJZ(operand); break;

        case 43:
            fHALT(); break;

        default:
            break;
        }
    }

    show_GUI(bg_color, fg_color);

    return 0;
}
