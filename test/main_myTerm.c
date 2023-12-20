#include "../src/myTerm.c"

int main()
{
    printf("123\n");
    mt_clrscr();
    printf("Экран очищен\n");

    int x = 5;
    int y = 10;
    mt_gotoXY(x,y);
    printf("Переход в позицию %d;%d\n", x, y);

    int row, col;
    mt_getscreensize(&row, &col);
    printf("rows = %d, cols = %d\n", row, col);

    printf("Синий текст: ");
    mt_setfgcolor(BLUE);
    printf("BLUE\n");

    printf("Синий текст на голубом фоне: ");
    mt_setbgcolor(CYAN);
    printf("BLUE ON CYAN\n");

    return 0;
}