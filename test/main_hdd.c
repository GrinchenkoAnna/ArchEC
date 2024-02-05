#include <stdio.h>

#include "../src/hdd.c"

int main()
{
    tLBA lba = 10018890;
    tCHS chs = { 0, 0, 0 };
    g_lba2chs(lba, &chs);
    printf("%u/%u/%u\n", chs.cylinder, chs.head, chs.sector);

    return 0;
}

