#include "../src/myBigChars.c"
#include "../src/myTerm.c"

int main()
{
    printf("123\n");
    mt_clrscr();
    printf("Экран очищен\n");

    char str[5] = "Hello";
    printf("%s: ", str);
    bc_printA(str);

    printf("\n");
    bc_box(2, 10, 10, 50);

    int bigchar[2] = {-2122213018, 405029505};
    //123456789 = 10000001 10000001 10011001 01100110
    //987654321 = 00011000 00100100 01000010 10000001
    bc_printbigchar(bigchar, 15, 2, RED, LIGHT_GRAY); //выводит 7 строчек, а не 8
    printf("\n\n");

    bc_setbigcharpos(bigchar, 3, 2, 1);
    printf("%d\n", bigchar[0]);
    bc_printbigchar(bigchar, 15, 2, RED, LIGHT_GRAY);

    return 0;
}