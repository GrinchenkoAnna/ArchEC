#include "library.c"
#include "myTerm.c"
#include "myBigChars.c"

void print_memory()
{
    int x = 2, y = 2;

    bc_box(0, 0, 12, 60);
    mt_gotoXY(0, 28);
    printf(" Memory ");

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        mt_gotoXY(x, y);
        if (sc_memory[i] != 0) 
        {                 
            mt_setfgcolor(GREEN);
            printf("+%.4d", sc_memory[i]);
            mt_setfgcolor(WHITE);
        }
        else { printf("+%.4d", sc_memory[i]); } 

        y += 6;
        if (i%10 == 9) { x++; y = 2; }
    }
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
    int bigchar[2];

    bc_box(12, 0, 22, 43);
    
}

int main()
{
    mt_clrscr();
    mt_setbgcolor(BLACK);
    mt_setfgcolor(WHITE);

    sc_memorySet(50, 9999); //для примера
    print_memory();

    sc_regSet(OPERATION_OVERFLOW, 1); //для примера
    print_flags();

    print_keys();

    int instructionCounter = 50; //адрес ячейки памяти
    print_bigChar(50);

    mt_gotoXY(23, 0);

    return 0;
}