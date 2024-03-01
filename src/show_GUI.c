#include "show_GUI.h"

dye fg_color;
dye bg_color;

void print_memory(dye bg_color, dye fg_color)
{
    int x = 2, y = 2;
    int value = 0;

    bc_box(0, 0, 12, 60);
    mt_gotoXY(0, 28);
    printf(" Memory ");

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        sc_memoryGet(i, &value);

        mt_gotoXY(x, y);
        if (i == instructionCounter)
        {
            mt_setfgcolor(GREEN);
            if (value >= 0) { printf("+"); }
            printf("%.4d", value);
            mt_setfgcolor(fg_color);
        }
        else
        {
            if (value >= 0) { printf("+"); }
            printf("%.4d", value);
        }

        y += 6;
        if (i%10 == 9) { x++; y = 2; }
    }
}

void print_accumulator()
{
    bc_box(0, 62, 3, 84);
    mt_gotoXY(1, 67);
    printf(" accumulator ");
    mt_gotoXY(2, 71);
    if (accumulator >= 0) { printf("+"); }
    printf("%.4d", accumulator);
}

void print_instructionCounter()
{
    bc_box(3, 62, 6, 84);
    mt_gotoXY(4, 63);
    printf(" instructionCounter ");
    mt_gotoXY(5, 71);
    if (instructionCounter >= 0) { printf("+"); }
    else { printf("-"); }
    printf("%.4d", instructionCounter);
}

void print_operation()
{
    bc_box(6, 62, 9, 84);
    mt_gotoXY(7, 68);
    printf(" Operation ");
    mt_gotoXY(8, 69);

    int value;
    sc_memoryGet(0, &value);
    if (value > 0)
    {
        int command, operand;
        sc_commandDecode(value, &command, &operand);
        printf("+%.2x : %.2x", command, operand);
    }
    else { printf("+00 : 00"); }
}

void print_flags(dye fg_color)
{
    int value;
    char flags[5] = {'O', '0', 'M', 'T', 'E'};

    bc_box(9, 62, 12, 84);
    mt_gotoXY(10, 70);
    printf(" Flags ");

    mt_gotoXY(11, 69);
    for (int i = 0; i < 5; i++)
    {
        sc_regGet(i, &value);
        if (value == 0) { printf("%c ", flags[i]); }
        else if (value > 0)
        {
            mt_setfgcolor(RED);
            printf("%c ", flags[i]);
            mt_setfgcolor(WHITE);
        }
    }
}

void print_keys()
{
    bc_box(12, 45, 22, 84);
    mt_gotoXY(13, 46);
    printf(" Keys: ");

    mt_gotoXY(14, 45);
    printf("│l   - load\n");
    mt_gotoXY(15, 45);
    printf("│s   - save\n");
    mt_gotoXY(16, 45);
    printf("│r   - run\n");
    mt_gotoXY(17, 45);
    printf("│t   - step\n");
    mt_gotoXY(18, 45);
    printf("│i   - reset\n");
    mt_gotoXY(19, 45);
    printf("│F5  - accumulator\n");
    mt_gotoXY(20, 45);
    printf("│F6  - instructionCounter\n");
}

void print_bigChar(int memory_address, dye bg_color, dye fg_color)
{
    int value;

    bc_box(12, 0, 22, 43);
    if (sc_memoryGet(memory_address, &value) == -1) return;

    if (value >= 0) { bc_printbigchar(bigchar_plus, 14, 2, fg_color, bg_color); }
    else if (value < 0)
    {
        bc_printbigchar(bigchar_minus, 14, 2, fg_color, bg_color);
        value *= -1;
    }
    for (int i = 0; i < 4; i++)
    {
        switch (value/(int)pow(10, 3-i))
        {
            case 0:
                bc_printbigchar(bigchar_0, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 1:
                bc_printbigchar(bigchar_1, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 2:
                bc_printbigchar(bigchar_2, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 3:
                bc_printbigchar(bigchar_3, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 4:
                bc_printbigchar(bigchar_4, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 5:
                bc_printbigchar(bigchar_5, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 6:
                bc_printbigchar(bigchar_6, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 7:
                bc_printbigchar(bigchar_7, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 8:
                bc_printbigchar(bigchar_8, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            case 9:
                bc_printbigchar(bigchar_9, 14, 2+8*(i+1), fg_color, bg_color);
                break;

            default: break;
        }
        mt_gotoXY(25 + i, 0);
        value %= (int)pow(10, 3-i);
    }
}

void show_GUI(dye bg_color, dye fg_color)
{
    mt_clrscr();
    mt_setbgcolor(bg_color);
    mt_setfgcolor(fg_color);

    print_memory(bg_color, fg_color);
    print_accumulator();
    print_instructionCounter();
    print_operation();
    print_flags(fg_color);
    print_keys();
    print_bigChar(instructionCounter, bg_color, fg_color);

    mt_gotoXY(32, 0);
}
