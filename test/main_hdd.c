#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../src/hdd.c"

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

    //элемент таблицы разделов
    typedef struct pt_entry
    {
        unsigned char bootable;       //флаг активности раздела
        tCHS start_part;              //CHS адрес начального сектора раздела
        unsigned char type_part;      //системный идентификатор (кодовый идентификатор ОС)
        tCHS end_part;                //CHS адрес конечного сектора раздела
        unsigned int sect_before;     //LBA адрес начального сектора (число секторов перед разделом)
        unsigned int sect_total;      //размер раздела в секторах
        long size;                    //размер раздела
        tLBA address;                 //адрес начала раздела
        struct pt_entry *next;
    } pt_entry_t;

    pt_entry_t *head = (pt_entry_t*)malloc(sizeof(pt_entry_t));
    head = NULL;
    //pt_entry_t *tail = NULL;

    tLBA start = 1;
    tLBA end;
    /* tCHS start_part  = { 0, 0, 1 }; */
    /* tCHS end_part; */

    //формирование таблицы разделов
    int entered_value;
    tIDECHS idechs_hdd;
    uint16_t max_cylinders = 65535;
    uint8_t max_heads = 15;
    uint8_t max_sectors = 255;
    tLBA lba_hdd;
    tCHS chs_hdd;
    bool active = false;

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

    //убрать ненужное
    g_idechs2chs(idechs_hdd, &chs_hdd);
    printf("chs geom: %d/%d/%d\n", chs_hdd.cylinder, chs.head, chs.sector);
    g_idechs2lba(idechs_hdd, &lba_hdd);

    long double hdd_size = (long double)(idechs_hdd.cylinder * idechs_hdd.head * idechs_hdd.sector)/(1024 * 1024);
    printf("Размер диска: %.2Lf Gb\n", hdd_size);

    //цикл для всех записей
    printf("Введите размер требуемого раздела на диске (Kb):\n");
    scanf("%d", &entered_value);
    pt_entry_t *part = (pt_entry_t*)malloc(sizeof(pt_entry_t));
    if (entered_value <= hdd_size*1024*1024) //пока - самый первый раздел
    {
        part->size = entered_value;
        part->address = start;
        part->sect_before = start;
        a_lba2chs(chs_hdd, part->address, &part->start_part);
        end = start + part->size;
        a_lba2chs(chs_hdd, end, &part->end_part);
        part->sect_total = part->end_part.sector - part->start_part.sector;

enter_os:
        printf("Введите тип ОС (№):\n");
        printf(" 1. Empty\n");
        printf(" 2. FAT12\n");
        printf(" 3. FAT16 <32M\n");
        printf(" 4. Расширенный\n");
        printf(" 5. MS-DOS FAT16\n");
        printf(" 6. HPFS/NTFS\n");
        printf(" 7. Win95 FAT32 (LBA)\n");
        printf(" 8. Win95 FAT16\n");
        printf(" 9. Linux swap\n");
        printf("10. Linux\n");
        scanf("%d", &entered_value);
        switch (entered_value)
        {
        case 1:
            part->type_part = 0x00;
            break;

        case 2:
            part->type_part = 0x01;
            break;

        case 3:
            part->type_part = 0x04;
            break;

        case 4:
            part->type_part = 0x05;
            break;

        case 5:
            part->type_part = 0x06;
            break;

        case 6:
            part->type_part = 0x07;
            break;

        case 7:
            part->type_part = 0x0c;
            break;

        case 8:
            part->type_part = 0x0e;
            break;

        case 9:
            part->type_part = 0x82;
            break;

        case 10:
            part->type_part = 0x83;

        default:
            printf("Error: wrong OS type. Repeat the input.\n");
            goto enter_os;
            break;
        }

        if (!active)
        {
            //поправить - пропускает (что-то в буфере?)
            printf("Будет ли данный раздел активным? (y/n)\n");
            char choice;
            scanf("%c", &choice);
            scanf("%c", &choice);
            //отправлять пользователя вводить заново, если он ввел что попало
            if (choice == 'y')
            {
                part->bootable = 128;
                active = true;
            }
            else { part->bootable = 0; }
        }

        //для расширенного раздела - разработать алгоритм

        start += part->size;
        hdd_size -= part->size;

        part->next = head;
        head = part;
    }

    //цикл для всех записей
    pt_entry_t *temp = (pt_entry_t*)malloc(sizeof(pt_entry_t));
    temp = head;
    printf("Активный | CHS начало | Сист. ID | CHS конец  | LBA начало        | Размер (сект.)\n");
    printf("---------+------------+----------+------------+-------------------+---------------\n");
    printf("%-3x      | %-.4u/%-.2u/%-.2u | %-4x     | %-.4u/%-.2u/%-.2u | %-10d        | %-10d\n",
           temp->bootable,
           temp->start_part.cylinder, temp->start_part.head, temp->start_part.sector,
           temp->type_part,
           temp->end_part.cylinder, temp->end_part.head, temp->end_part.sector,
           temp->sect_before,
           temp->sect_total);


    return 0;
}

