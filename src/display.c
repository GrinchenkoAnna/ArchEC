#include <math.h>

#include "library.c"
#include "myTerm.c"
#include "myBigChars.c"
#include "myReadkey.c"

short instructionCounter = 0;
unsigned int accumulator = 0;

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
        if (value != 0)
        {                 
            mt_setfgcolor(GREEN);
            printf("+%.4d", value);
            mt_setfgcolor(WHITE);
        }
        else { printf("+%.4d", value); }

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
    else { printf("-"); }
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
    char flags[5] = {'O', 'D', 'M', 'C', 'I'};

    bc_box(9, 62, 12, 84);
    mt_gotoXY(10, 70);
    printf(" Flags ");

    mt_gotoXY(11, 69);
    for (int i = 0; i < 5; i++)
    {
        if (sc_regGet(i, &value) == 0) { printf("%c ", flags[i]); }
        else if (sc_regGet(i, &value) == 1) 
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
    if (sc_memoryGet(memory_address, &value) != 0) return;

    if (value > 0) { bc_printbigchar(bigchar_plus, 14, 2, WHITE, BLACK); }
    else if (value < 0) { bc_printbigchar(bigchar_minus, 14, 2, WHITE, BLACK); }
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
    print_bigChar(accumulator);


    mt_gotoXY(30, 0);
}

void key_convert(enum keys key)
{
    switch (key)
    {
    case KEY_l:
        sc_memoryLoad(filename);
        break;

    case KEY_s:
        sc_memorySave(filename);
        break;

    case KEY_r:

        break;

    case KEY_F5:

        break;

    case KEY_i:

        break;

    case KEY_up:

        break;

    case KEY_down:

        break;

    case KEY_left:

        break;

    case KEY_right:

        break;

    case KEY_t:
        mt_gotoXY(30, 0);
        rk_mytermrestore();
        printf("Accumulator > ");
        rk_mytermregime(0, 0, 5, 1, 1);
        printf("Accumulator > ");

        char buffer[5];
        int terminal = open("/dev/tty", O_RDWR);
        read(terminal, buffer, 5);

        sscanf(buffer, "%x", &accumulator);
        rk_mytermregime(0, 0, 2, 0, 1);
        break;

    case KEY_F6:

        break;

    case KEY_enter:

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

    sc_regInit();
    sc_memorySet(26, 2020);
    sc_memorySave(filename);
    sc_memoryInit();

    rk_mytermsave();
    rk_mytermregime(0, 0, 2, 1, 1);

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
