#include <stdio.h>

#include "../src/hdd.c"

int main()
{
    printf(">>Geometry\n");
    printf("LBA->CHS\n");
    tLBA lba = 10018890;
    tCHS chs = { 0, 0, 0 };
    g_lba2chs(lba, &chs);
    printf("lba - %u, chs - %u/%u/%u\n", lba, chs.cylinder, chs.head, chs.sector);

    printf("\nLBA->Large\n");
    lba = 10018890;
    tLARGE large = { 0, 0, 0 };
    g_lba2large(lba, &large);
    printf("lba - %u, large - %u/%u/%u\n", lba, large.cylinder, large.head, large.sector);

    printf("\nLBA->IDECHS\n");
    lba = 10018890;
    tIDECHS idechs = { 0, 0, 0 };
    g_lba2idechs(lba, &idechs);
    printf("lba - %u, idechs - %u/%u/%u\n", lba, idechs.cylinder, idechs.head, idechs.sector);

    printf("\nCHS->Large\n");
    chs.cylinder = 10602;
    chs.head = 15;
    chs.sector = 63;
    g_chs2large(chs, &large);
    printf("chs - %u/%u/%u, large - %u/%u/%u\n", chs.cylinder, chs.head, chs.sector, large.cylinder, large.head, large.sector);

    printf("\nCHS->IDECHS\n");
    chs.cylinder = 10602;
    chs.head = 15;
    chs.sector = 63;
    g_chs2idechs(chs, &idechs);
    printf("chs - %u/%u/%u, idechs - %u/%u/%u\n", chs.cylinder, chs.head, chs.sector, idechs.cylinder, idechs.head, idechs.sector);

    printf("\nLarge->CHS\n");
    large.cylinder = 1252;
    large.head = 128;
    large.sector = 63;
    g_large2chs(large, &chs);
    printf("large - %u/%u/%u, chs - %u/%u/%u\n", large.cylinder, large.head, large.sector, chs.cylinder, chs.head, chs.sector);


    printf("\nLarge->IDECHS\n");
    large.cylinder = 1252;
    large.head = 128;
    large.sector = 63;
    g_large2idechs(large, &idechs);
    printf("large - %u/%u/%u, idechs - %u/%u/%u\n", large.cylinder, large.head, large.sector, idechs.cylinder, idechs.head, idechs.sector);

    printf("\nCHS->LBA\n");
    chs.cylinder = 10602;
    chs.head = 15;
    chs.sector = 63;
    g_chs2lba(chs, &lba);
    printf("chs - %u/%u/%u, lba - %u\n", chs.cylinder, chs.head, chs.sector, lba);

    printf("\nLarge->LBA\n");
    large.cylinder = 1252;
    large.head = 128;
    large.sector = 63;
    g_large2lba(large, &lba);
    printf("large - %u/%u/%u, lba - %u\n", large.cylinder, large.head, large.sector, lba);

    printf("\nIDECHS->LBA\n");
    idechs.cylinder = 39289;
    idechs.head = 1;
    idechs.sector = 255;
    g_idechs2lba(idechs, &lba);
    printf("chs - %u/%u/%u, lba - %u\n", idechs.cylinder, idechs.head, idechs.sector, lba);

    printf("\n\n>>Adresses\n");
    tCHS geometry = { 10602, 15, 63 };
    printf("LBA->CHS\n");
    a_lba2chs(geometry, lba, &chs);
    printf("geometry - %u, %u, %u\nlba - %u, chs - %u/%u/%u\n", geometry.cylinder, geometry.head, geometry.sector, lba, chs.cylinder, chs.head, chs.sector);

    printf("\nLBA->CHS\n");
    a_chs2lba(geometry, chs, &lba);
    printf("geometry - %u, %u, %u\nchs - %u/%u/%u, lba - %u\n", geometry.cylinder, geometry.head, geometry.sector, chs.cylinder, chs.head, chs.sector, lba);


    return 0;
}

