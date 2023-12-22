#include "library.c"
#include "myTerm.c"

void print_memory()
{

    printf("┌");
    for (int i = 0; i < 25; i++) { printf("─"); }
    printf(" Memory ");
    for (int i = 0; i < 26; i++) { printf("─"); }
    printf("┐\n");

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (i%10 == 0) { printf("│"); }
        if (i%10 == 9) 
        { 
            if (sc_memory[i] != 0) 
            {                 
                mt_setfgcolor(GREEN);
                printf("+%.4d", sc_memory[i]);
                mt_setfgcolor(LIGHT_GRAY);
            }
            else { printf("+%.4d", sc_memory[i]); }             
            printf("│\n"); 
        }
        else 
        { 
            if (sc_memory[i] != 0) 
            { 
                mt_setfgcolor(GREEN);
                printf("+%.4d ", sc_memory[i]);
                mt_setfgcolor(LIGHT_GRAY);
            }
            else { printf("+%.4d ", sc_memory[i]); }   
        }        
    }

    printf("└");
    for (int i = 0; i < 59; i++) { printf("─"); }
    printf("┘\n");
}

void print_flags()
{
    int value;
    char flags[5] = {'O', 'D', 'M', 'C', 'I'};
    mt_gotoXY(11, 62);

    printf("┌");
    for (int i = 0; i < 7; i++) { printf("─"); }
    printf(" Flags ");
    for (int i = 0; i < 7; i++) { printf("─"); }    
    printf("┐");

    mt_gotoXY(12, 62);
    printf("│      ");
    for (int i = 0; i < 5; i++)
    {
        if (sc_regGet(i, &value) == 0) { printf("%c ", flags[i]); }
        else if (sc_regGet(i, &value) == 1) 
        {
            mt_setfgcolor(RED); 
            printf("%c ", flags[i]); 
            mt_setfgcolor(LIGHT_GRAY); 
        }
    }
    printf("     │");

    mt_gotoXY(13, 62);
    printf("└");
    for (int i = 0; i < 21; i++) { printf("─"); }
    printf("┘");

   mt_gotoXY(14, 0);
}

void print_keys()
{
    mt_gotoXY(14, 45);
    printf("┌");    
    printf(" Keys: ");
    for (int i = 0; i < 31; i++) { printf("─"); }    
    printf("┐\n");

    mt_gotoXY(15, 45);
    printf("│l   - load                            │\n");
    mt_gotoXY(16, 45);
    printf("│s   - save                            │\n");
    mt_gotoXY(17, 45);
    printf("│r   - run                             │\n");
    mt_gotoXY(18, 45);
    printf("│t   - step                            │\n");
    mt_gotoXY(19, 45);
    printf("│i   - reset                           │\n");
    mt_gotoXY(20, 45);
    printf("│F5  - accumulator                     │\n");
    mt_gotoXY(21, 45);
    printf("│F6  - instructionCounter              │\n");
    mt_gotoXY(22, 45);
    printf("│                                      │\n");

    mt_gotoXY(23, 45);
    printf("└");    
    for (int i = 0; i < 38; i++) { printf("─"); }    
    printf("┘\n");
}

int main()
{
    sc_memorySet(50, 9999); //для примера
    print_memory();

    sc_regSet(OPERATION_OVERFLOW, 1); //для примера
    print_flags();

    print_keys();

    return 0;
}