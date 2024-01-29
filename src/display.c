#include <math.h>

#include "library.c"
#include "myTerm.c"
#include "myBigChars.c"
#include "myReadkey.c"

void print_memory()
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
            mt_setfgcolor(WHITE);
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

void print_flags()
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

void print_bigChar(int memory_address)
{
    int value;

    bc_box(12, 0, 22, 43);
    if (sc_memoryGet(memory_address, &value) == -1) return;

    if (value >= 0) { bc_printbigchar(bigchar_plus, 14, 2, WHITE, BLACK); }
    else if (value < 0)
    {
        bc_printbigchar(bigchar_minus, 14, 2, WHITE, BLACK);
        value *= -1;
    }
    for (int i = 0; i < 4; i++)
    {
        switch (value/(int)pow(10, 3-i))
        {
            case 0:
                bc_printbigchar(bigchar_0, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 1:
                bc_printbigchar(bigchar_1, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 2:
                bc_printbigchar(bigchar_2, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 3:
                bc_printbigchar(bigchar_3, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 4:
                bc_printbigchar(bigchar_4, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 5:
                bc_printbigchar(bigchar_5, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 6:
                bc_printbigchar(bigchar_6, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 7:
                bc_printbigchar(bigchar_7, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 8:
                bc_printbigchar(bigchar_8, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            case 9:
                bc_printbigchar(bigchar_9, 14, 2+8*(i+1), WHITE, BLACK);
                break;

            default: break;
        }
        mt_gotoXY(25 + i, 0);
        value %= (int)pow(10, 3-i);
    }
}

void show_GUI()
{
    mt_clrscr();
    mt_setbgcolor(BLACK);
    mt_setfgcolor(WHITE);

    print_memory();
    print_accumulator();
    print_instructionCounter();
    print_flags();
    print_keys();
    print_bigChar(instructionCounter);

    mt_gotoXY(32, 0);
}

void key_convert(enum keys key)
{
    char buffer[10];
    int terminal = open("/dev/tty", O_RDWR);

    switch (key)
    {
    case KEY_l:
        sc_memoryLoad(filename);
        break;

    case KEY_s:
        sc_memorySave(filename);
        break;

    case KEY_r:
        sc_regSet(IGNORING_CLOCK_PULSES, 0);
        break;

    case KEY_t:

        break;

    case KEY_i:
        sc_memoryInit();
        sc_regInit();
        break;

    case KEY_up:
        if (instructionCounter - 10 >= 0) { instructionCounter -= 10; }
        else { instructionCounter += 90; }
        break;

    case KEY_down:
        if (instructionCounter + 10 <= 99) { instructionCounter += 10; }
        else { instructionCounter -= 90; }
        break;

    case KEY_left:
        if (instructionCounter - 1 >= 0) { instructionCounter--; }
        else { instructionCounter = 99; }
        break;

    case KEY_right:
        if (instructionCounter + 1 <= 99) { instructionCounter++; }
        else { instructionCounter = 0; }
        break;

    case KEY_F5:
        read(terminal, &buffer, sizeof(buffer));

        if (buffer[0] == '-')
        {
            accumulator = atoi(buffer + 1);
            if (accumulator > 0) { accumulator *= -1; }
        }
        else if (buffer[0] == '+' || buffer[0] == '0' || buffer[0] == '1' ||
                 buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4' ||
                 buffer[0] == '5' || buffer[0] == '6' || buffer[0] == '7' ||
                 buffer[0] == '8' || buffer[0] == '9')
        {
            if (buffer[0] == '+') { accumulator = atoi(buffer + 1); }
            else { accumulator = atoi(buffer); }

            if (accumulator < 0) { accumulator *= -1; }
        }

        if (accumulator > 9999 || accumulator < -9999)
        {
            sc_regSet(OPERATION_OVERFLOW, 1);
            if (accumulator > 0) { accumulator = 9999; }
            else { accumulator = -9999; }
        }
        else { sc_regSet(OPERATION_OVERFLOW, 0); }

        break;

    case KEY_F6:

        break;

    case KEY_enter:
        mt_gotoXY(24, 0);
        printf("Enter the value: ");
        read(terminal, &buffer, sizeof(buffer));

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

        sc_memorySet(instructionCounter, value);

        break;

    case KEY_default:
        printf("Unknown command\n");
        break;

    case KEY_quit: break;
    }
}

int main()
{
    enum keys key;

    sc_memorySet(26, 2020);
    sc_memorySave(filename);
    sc_memoryInit();
    sc_regInit();

    rk_mytermsave();
    rk_mytermregime(1, 0, 0, 1, 1);

    show_GUI();

    do
    {
        rk_readkey(&key);
        key_convert(key);
        show_GUI();
    } while (key != KEY_quit);

    mt_clrscr();
    printf("Exit My Simple Computer. Terminal settings restored\n");
    rk_mytermrestore();

    return 0;
}
