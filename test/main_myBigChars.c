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
    

    return 0;
}