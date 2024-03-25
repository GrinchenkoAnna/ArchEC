#include "CU.h"

int fREAD(int operand)
{
    mt_printtoterm("Input:< ");
    char buffer[8];
    mt_readfromterm(buffer, sizeof(buffer));

    int value;
    if (buffer[0] == '-')
    {
        value = atoi(buffer + 1);
        value *= -1;
    }
    else if (buffer[0] == '+' || buffer[0] == '0' || buffer[0] == '1' ||
             buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4' ||
             buffer[0] == '5' || buffer[0] == '6' || buffer[0] == '7' ||
             buffer[0] == '8' || buffer[0] == '9')
    {
         if (buffer[0] == '+') { value = atoi(buffer + 1); }
         else { value = atoi(buffer); }
    }

    sc_memorySet(operand, value);
    return 0;
}

int fWRITE(int operand)
{
    int value;
    char buffer[5];
    sc_memoryGet(operand, &value);
    sprintf(buffer, " %d", value);
    mt_printtoterm("Output:>");
    mt_printtoterm(buffer);

    //задержка, чтобы показать вывод
    mt_printtoterm("\nPress any key");
    mt_readfromterm(buffer, sizeof(buffer));
    mt_enter_alt_charset_mode();

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
    { ALU(command, operand); }
    else
    {
        switch (command)
        {
        case 10: fREAD(operand); break;
        case 11: fWRITE(operand); break;
        case 20: fLOAD(operand); break;
        case 21: fSTORE(operand); break;
        case 40: fJUMP(operand); break;
        case 41: fJNEG(operand); break;
        case 42: fJZ(operand); break;
        case 43: fHALT(); break;
        default: break;
        }
    }

    return 0;
}
