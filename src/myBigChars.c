#include "myBigChars.h"

int bc_printA(char *str)
{
    printf("\E(0");
    printf("%s", str);
    printf("\E(B");

    return 0;
}

int bc_box(int x1, int y1, int x2, int y2)
{
    if ((x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) || (x2 < x1 || y2 < y1))
    { return -1; }

    printf("\E(0");

    for (int i = 0; i < x2 - x1; i++)
    {
        mt_gotoXY(x1 + i + 1, y1);
        if (i == 0)
        {
            printf("l");
            for (int j = 0; j < y2 - y1 - 1; j++) { printf("q"); }
            printf("k\n");
        }
        else if (i == x2 - x1 - 1)
        {
            printf("m");
            for (int j = 0; j < y2 - y1 - 1; j++) { printf("q"); }
            printf("j\n");
        }
        else
        {
            printf("x");
            for (int j = 1; j < y2 - y1; j++) { printf(" "); }
            printf("x\n");
        }
    }
    printf("\E(B");

    return 0;
}

int bc_printbigchar(int bigchar[2], int x, int y, dye color, dye background)
{
    if (x < 0 || y < 0) { return -1; }

    printf("\E(0");

    for (int i = 0; i < 2; i++)
    {
        mt_gotoXY(x + i * 4, y);
        for (int byte_of_number = 0; byte_of_number < 4; byte_of_number++)
        {
            for (int j = 1; j <= BIGCHARSIZE; j++)
            {
                mt_setfgcolor(color);
                mt_setbgcolor(background);

                if ((bigchar[i] >> ((BIGCHARSIZE - j) + 8 * byte_of_number)) & 0x1)
                { printf("a"); }
                else { printf(" "); }

                mt_resetcolor();
            }
            printf("\n");
            mt_gotoXY(x + i * 4 + byte_of_number + 1, y);
        }
    }
    printf("\E(B");

    return 0;
}

int bc_setbigcharpos(int *bigchar, int x, int y, int value)
{
    if (value < 0 || !(y < 8 && y >= 0) || !(x < 8 && x >= 0)) { return -1; }

    int offset = ((y - 1) * 8 + 8 - x);
    if (value) { bigchar[y / 5] |= 1 << offset; }
    else { bigchar[y / 5] &= ~(1 << offset); }

    return 0;
}

int bc_getbigcharpos(int *bigchar, int x, int y, int *value)
{
    if (value < 0 || !(y < 8 && y >= 0) || !(x < 8 && x >= 0)) { return -1; }

    *value = (bigchar[y / 5] >> ((y - 1) * 8 + 8 - x)) & 0x1;

    return 0;
}

int bc_bigcharwrite(int fd, int *bigchar, int count)
{   
    int n = write(fd, bigchar, sizeof(bigchar)*count);
    if (n > 0) { return 0; }
    else { return -1; }
}

int bc_bigcharread(int fd, int *bigchar, int need_count, int *count)
{
    lseek(fd, 0, SEEK_SET);
    *count = read(fd, bigchar, sizeof(bigchar)*need_count)/8;    
    if (*count > 0) { return 0; }
    else { return -1; }
}
