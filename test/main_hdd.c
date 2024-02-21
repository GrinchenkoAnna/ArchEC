#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../src/hdd.c"

//элемент таблицы разделов
typedef struct pt_entry
{
    unsigned int bootable;        //флаг активности раздела
    tCHS start_part;              //CHS адрес начального сектора раздела
    unsigned int type_part;       //системный идентификатор (кодовый идентификатор ОС)
    tCHS end_part;                //CHS адрес конечного сектора раздела
    unsigned int sect_before;     //LBA адрес начального сектора (число секторов перед разделом)
    unsigned int sect_total;      //размер раздела в секторах
    long size;                    //размер раздела
    tLBA address;                 //адрес начала раздела
} pt_entry_t;

pt_entry_t *pttable = NULL;

tLBA start = 1;
tLBA end;

//формирование таблицы разделов
int entered_value;
tIDECHS idechs_hdd;
uint16_t max_cylinders = 65535;
uint8_t max_heads = 15;
uint8_t max_sectors = 255;
tLBA lba_hdd;
tCHS chs_hdd;
bool active = false;
bool extended = false;
long part_size;


void create_partition_table(unsigned long long int hdd_size, pt_entry_t *pttable)
{
    int parts = -1;
    pttable = malloc(sizeof(pt_entry_t));
    while (hdd_size > 0)
    {
        printf("Доступно: %llu Kb\n", hdd_size);
        parts++;
        printf("Введите размер требуемого раздела на диске (Kb):\n");
    scanf("%d", &entered_value);
        if (entered_value == 0) { break; }
        pttable = realloc(pttable, sizeof(pt_entry_t)*(parts + 1));

        part_size = entered_value;
        pttable[parts].size = entered_value;
        pttable[parts].address = start;
        pttable[parts].sect_before = start;
        a_lba2chs(chs_hdd, pttable[parts].address, &pttable[parts].start_part);
        end = start + part_size;
        a_lba2chs(chs_hdd, end, &pttable[parts].end_part);
        pttable[parts].sect_total = pttable[parts].end_part.sector - pttable[parts].start_part.sector;
        if (pttable[parts].sect_total < 0) { pttable[parts].sect_total *= -1; }

enter_os:
        printf("Введите тип ОС (№):\n");
        printf(" 0. Empty\n");
        printf(" 1. FAT12\n");
        printf(" 2. FAT16 <32M\n");
        printf(" 3. Расширенный\n");
        printf(" 4. MS-DOS FAT16\n");
        printf(" 5. HPFS/NTFS\n");
        printf(" 6. Win95 FAT32 (LBA)\n");
        printf(" 7. Win95 FAT16\n");
        printf(" 8. Linux swap\n");
        printf(" 9. Linux\n");
        scanf("%d", &entered_value);
        switch (entered_value)
        {
        case 0:
            pttable[parts].type_part = 0x00;
            break;

        case 1:
            pttable[parts].type_part = 0x01;
            break;

        case 2:
            pttable[parts].type_part = 0x04;
            break;

        case 3:
            pttable[parts].type_part = 0x05;
            extended = true;
            pt_entry_t *pptable_extended = NULL;
            create_partition_table(pttable[parts].size, pptable_extended);
            extended = false;
            break;

        case 4:
            pttable[parts].type_part = 0x06;
            break;

        case 5:
            pttable[parts].type_part = 0x07;
            break;

        case 6:
            pttable[parts].type_part = 0x0c;
            break;

        case 7:
            pttable[parts].type_part = 0x0e;
            break;

        case 8:
            pttable[parts].type_part = 0x82;
            break;

        case 9:
            pttable[parts].type_part = 0x83;
            break;

        default:
            printf("Error: wrong OS type. Repeat the input.\n");
            goto enter_os;
            break;
        }

        if (!active)
        {
enter_bootable:
            printf("Будет ли данный раздел активным? (y/n)\n");
            char choice;
            scanf("%c", &choice);
            scanf("%c", &choice);

            if (choice == 'y')
            {
                pttable[parts].bootable = 128;
                active = true;
            }
            else if (choice == 'n') { pttable[parts].bootable = 0; }
            else
            {
                printf("Error: wrong answer. Repeat the input.\n");
                goto enter_bootable;
            }
        }
        else { pttable[parts].bootable = 0; }

        start += pttable[parts].size;
        hdd_size -= pttable[parts].size;
    }

    if (extended)
    {
        printf("Расширенная таблица разделов\n");
        parts++;
    }
    else
    { printf("Основная таблица разделов\n"); }
    printf("Активный | CHS начало | Сист. ID | CHS конец  | LBA начало        | Размер (сект.)\n");
    for (int i = 0; i < parts; i++)
    {
        printf("---------+------------+----------+------------+-------------------+---------------\n");
        printf("%-3x      | %-.4u/%-.2u/%-.2u | 0x%-4x   | %-.4u/%-.2u/%-.2u | %-10d        | %-10d\n",
        pttable[i].bootable,
        pttable[i].start_part.cylinder, pttable[i].start_part.head, pttable[i].start_part.sector,
        pttable[i].type_part,
        pttable[i].end_part.cylinder, pttable[i].end_part.head, pttable[i].end_part.sector,
        pttable[i].sect_before,
        pttable[i].sect_total);
    }
}

int main()
{
    printf(">>Geometry\n");
    printf("LBA->CHS\n");
    tLBA lba = 10018890;
    tCHS chs = { 0, 0, 0 };
    g_lba2chs(lba, &chs);
    printf("lba - %u, chs - %u/%u/%u\n",
           lba, chs.cylinder, chs.head, chs.sector);

    printf("\nLBA->Large\n");
    lba = 10018890;
    tLARGE large = { 0, 0, 0 };
    g_lba2large(lba, &large);
    printf("lba - %u, large - %u/%u/%u\n",
           lba, large.cylinder, large.head, large.sector);

    printf("\nLBA->IDECHS\n");
    lba = 10018890;
    tIDECHS idechs = { 0, 0, 0 };
    g_lba2idechs(lba, &idechs);
    printf("lba - %u, idechs - %u/%u/%u\n",
           lba, idechs.cylinder, idechs.head, idechs.sector);

    printf("\nCHS->Large\n");
    chs.cylinder = 10602;
    chs.head = 15;
    chs.sector = 63;
    g_chs2large(chs, &large);
    printf("chs - %u/%u/%u, large - %u/%u/%u\n",
           chs.cylinder, chs.head, chs.sector,
           large.cylinder, large.head, large.sector);

    printf("\nCHS->IDECHS\n");
    chs.cylinder = 10602;
    chs.head = 15;
    chs.sector = 63;
    g_chs2idechs(chs, &idechs);
    printf("chs - %u/%u/%u, idechs - %u/%u/%u\n",
           chs.cylinder, chs.head, chs.sector,
           idechs.cylinder, idechs.head, idechs.sector);

    printf("\nLarge->CHS\n");
    large.cylinder = 1252;
    large.head = 128;
    large.sector = 63;
    g_large2chs(large, &chs);
    printf("large - %u/%u/%u, chs - %u/%u/%u\n",
           large.cylinder, large.head, large.sector,
           chs.cylinder, chs.head, chs.sector);

    printf("\nLarge->IDECHS\n");
    large.cylinder = 1252;
    large.head = 128;
    large.sector = 63;
    g_large2idechs(large, &idechs);
    printf("large - %u/%u/%u, idechs - %u/%u/%u\n",
           large.cylinder, large.head, large.sector,
           idechs.cylinder, idechs.head, idechs.sector);

    printf("\nCHS->LBA\n");
    chs.cylinder = 10602;
    chs.head = 15;
    chs.sector = 63;
    g_chs2lba(chs, &lba);
    printf("chs - %u/%u/%u, lba - %u\n",
           chs.cylinder, chs.head, chs.sector, lba);

    printf("\nLarge->LBA\n");
    large.cylinder = 1252;
    large.head = 128;
    large.sector = 63;
    g_large2lba(large, &lba);
    printf("large - %u/%u/%u, lba - %u\n",
           large.cylinder, large.head, large.sector, lba);

    printf("\nIDECHS->LBA\n");
    idechs.cylinder = 39289;
    idechs.head = 1;
    idechs.sector = 255;
    g_idechs2lba(idechs, &lba);
    printf("chs - %u/%u/%u, lba - %u\n",
           idechs.cylinder, idechs.head, idechs.sector, lba);
/*--------------------------------------------------*/
    printf("\n\n>>Adresses\n");
    tCHS geometry_chs = { 1023, 15, 63 };
    printf("LBA->CHS\n");
    a_lba2chs(geometry_chs, lba, &chs);
    printf("geometry - %u/%u/%u\nlba - %u, chs - %u/%u/%u\n",
           geometry_chs.cylinder, geometry_chs.head, geometry_chs.sector,
           lba, chs.cylinder, chs.head, chs.sector);

    printf("\nLBA->Large\n");
    tLARGE geometry_large = { 1023, 255, 63 };
    a_lba2large(geometry_large, lba, &large);
    printf("geometry - %u/%u/%u\nlba - %u, large - %u/%u/%u\n",
           geometry_large.cylinder, geometry_large.head, geometry_large.sector,
           lba, large.cylinder, large.head, large.sector);

    printf("\nLBA->IDECHS\n");
    tIDECHS geometry_idechs = { 65535, 15, 255 };
    a_lba2idechs(geometry_idechs, lba, &idechs);
    printf("geometry - %u/%u/%u\nlba - %u, idechs - %u/%u/%u\n",
           geometry_idechs.cylinder, geometry_idechs.head, geometry_idechs.sector,
           lba, idechs.cylinder, idechs.head, idechs.sector);

    printf("\nCHS->LBA\n");
    a_chs2lba(geometry_chs, chs, &lba);
    printf("geometry - %u/%u/%u\nchs - %u/%u/%u, lba - %u\n",
           geometry_chs.cylinder, geometry_chs.head, geometry_chs.sector,
           chs.cylinder, chs.head, chs.sector, lba);

    printf("\nLarge->LBA\n");
    a_large2lba(geometry_large, large, &lba);
    printf("geometry - %u/%u/%u\nlarge - %u/%u/%u, lba - %u\n",
           geometry_large.cylinder, geometry_large.head, geometry_large.sector,
           large.cylinder, large.head, large.sector, lba);

    printf("\nIDECHS->LBA\n");
    a_idechs2lba(geometry_idechs, idechs, &lba);
    printf("geometry - %u/%u/%u\nchs - %u/%u/%u, lba - %u\n",
           geometry_idechs.cylinder, geometry_idechs.head, geometry_idechs.sector,
           idechs.cylinder, idechs.head, idechs.sector, lba);

    printf("\nLarge->CHS\n");
    a_large2chs(geometry_large, geometry_chs, large, &chs);
    printf("geometry_large - %u/%u/%u, geometry_chs - %u/%u/%u\nlarge - %u/%u/%u, chs - %u/%u/%u\n",
           geometry_large.cylinder, geometry_large.head, geometry_large.sector,
           geometry_chs.cylinder, geometry_chs.head, geometry_chs.sector,
           large.cylinder, large.head, large.sector,
           chs.cylinder, chs.head, chs.sector);

    printf("\nLarge->IDECHS\n");
    a_large2idechs(geometry_large, geometry_idechs, large, &idechs);
    printf("geometry_large - %u/%u/%u, geometry_idechs - %u/%u/%u\nlarge - %u/%u/%u, idechs - %u/%u/%u\n",
           geometry_large.cylinder, geometry_large.head, geometry_large.sector,
           geometry_idechs.cylinder, geometry_idechs.head, geometry_idechs.sector,
           large.cylinder, large.head, large.sector,
           idechs.cylinder, idechs.head, idechs.sector);

    printf("\nCHS->Large\n");
    a_chs2large(geometry_chs, geometry_large, chs, &large);
    printf("geometry_chs - %u/%u/%u, geometry_large - %u/%u/%u\nchs - %u/%u/%u, large - %u/%u/%u\n",
           geometry_chs.cylinder, geometry_chs.head, geometry_chs.sector,
           geometry_large.cylinder, geometry_large.head, geometry_large.sector,
           chs.cylinder, chs.head, chs.sector,
           large.cylinder, large.head, large.sector);

    printf("\nIDECHS->Large\n");
    a_idechs2large(geometry_idechs, geometry_large, idechs, &large);
    printf("geometry_idechs - %u/%u/%u, geometry_large - %u/%u/%u\nidechs - %u/%u/%u, large - %u/%u/%u\n",
           geometry_idechs.cylinder, geometry_idechs.head, geometry_idechs.sector,
           geometry_large.cylinder, geometry_large.head, geometry_large.sector,
           idechs.cylinder, idechs.head, idechs.sector,
           large.cylinder, large.head, large.sector);

    printf("\nCHS->IDECHS\n");
    a_chs2idechs(geometry_chs, geometry_idechs, chs, &idechs);
    printf("geometry_chs - %u/%u/%u, geometry_idechs - %u/%u/%u\nchs - %u/%u/%u, idechs - %u/%u/%u\n",
           geometry_chs.cylinder, geometry_chs.head, geometry_chs.sector,
           geometry_idechs.cylinder, geometry_idechs.head, geometry_idechs.sector,
           chs.cylinder, chs.head, chs.sector,
           idechs.cylinder, idechs.head, idechs.sector);

    printf("\nIDECHS->CHS\n");
    a_idechs2chs(geometry_idechs, geometry_chs, idechs, &chs);
    printf("geometry_idechs - %u/%u/%u, geometry_chs - %u/%u/%u\nidechs - %u/%u/%u, chs - %u/%u/%u\n",
           geometry_idechs.cylinder, geometry_idechs.head, geometry_idechs.sector,
           geometry_chs.cylinder, geometry_chs.head, geometry_chs.sector,
           idechs.cylinder, idechs.head, idechs.sector,
           chs.cylinder, chs.head, chs.sector);
/*--------------------------------------------------*/
    printf("\n\n>>Задание на лабораторную работу\n");

    printf("Введите геометрию диска в формате IDECHS:\n");

enter_c:
    printf("C: ");
    scanf("%d", &entered_value);
    if (entered_value > max_cylinders)
    {
        printf("Error: wrong number of cylinder. Repeat the input.\n");
        goto enter_c;
    }
    idechs_hdd.cylinder = entered_value;

enter_h:
    printf("H: ");
    scanf("%d", &entered_value);
    if (entered_value > max_heads)
    {
        printf("Error: wrong number of heads. Repeat the input.\n");
        goto enter_h;
    }
    idechs_hdd.head = entered_value;

enter_s:
    printf("S: ");
    scanf("%d", &entered_value);
    if (entered_value > max_sectors)
    {
        printf("Error: wrong number of sectors. Repeat the input.\n");
        goto enter_s;
    }
    idechs_hdd.sector = entered_value;

    g_idechs2chs(idechs_hdd, &chs_hdd);
    long double hdd_size = (long double)(idechs_hdd.cylinder * idechs_hdd.head * idechs_hdd.sector)/(1024 * 1024);
    printf("Размер диска: %.2Lf Gb\n", hdd_size);
    unsigned long long int hdd_size_kb = hdd_size*1024*1024;

    create_partition_table(hdd_size_kb, pttable);

    return 0;
}

