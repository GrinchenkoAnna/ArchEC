#include <stdio.h>

#include "../src/hdd.c"

int main()
{
    tLBA lba = 10018890;
    tCHS chs = { 0, 0, 0 };
    g_lba2chs(lba, &chs);
    printf("lba - %u, chs - %u/%u/%u\n", lba, chs.cylinder, chs.head, chs.sector);

    chs.cylinder = 623;
    chs.head = 255;
    chs.sector = 63;
    g_chs2lba(chs, &lba);
    printf("chs - %u/%u/%u, lba - %u\n", chs.cylinder, chs.head, chs.sector, lba);



    return 0;
}

