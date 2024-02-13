#include "hdd.h"

/* Возможно, все неправильно */

/* geometry */
int g_lba2chs(tLBA lba, tCHS *chs)
{
    chs->sector = 63;

    int L1 = lba/63;
    int logic_heads = L1/1023;
    for (int i = 0; i < 8; i++)
    {
        if (logic_heads < LOG_H[i])
        {
            logic_heads = LOG_H[i];
            break;
        }
    }

    int K = 2;
    while (logic_heads / K > 15) { K *= 2; }

    chs->head = logic_heads/K;
    chs->cylinder = L1/chs->head;

    return 0;
}

//изучить и исправить
int g_lba2large(tLBA lba, tLARGE *large)
{
    large->sector = 63;

    int L1 = lba/63;
    int logic_heads = L1/1023;
    for (int i = 0; i < 8; i++)
    {
        if (logic_heads < LOG_H[i])
        {
            logic_heads = LOG_H[i];
            break;
        }
    }

    int K = 2;
    while (logic_heads / K > 255) { K *= 2; }

    large->head = logic_heads/K;
    large->cylinder = L1/large->head;

    return 0;
}

int g_lba2idechs(tLBA lba, tIDECHS *idechs)
{
    idechs->sector = 255;

    int L1 = lba/255;
    int logic_heads = L1/65535;
    for (int i = 0; i < 8; i++)
    {
        if (logic_heads < LOG_H[i])
        {
            logic_heads = LOG_H[i];
            break;
        }
    }

    int K = 2;
    while (logic_heads / K > 15) { K *= 2; }

    idechs->head = logic_heads/K;
    idechs->cylinder = L1/idechs->head;

    return 0;
}

//изучить и исправить
int g_chs2large(tCHS chs, tLARGE *large)
{
    int cylinders = chs.cylinder;
    int heads = chs.head;

    large->sector = chs.sector;

    while (cylinders/2 > 0 && heads*2 <= 256)
    {
        cylinders /= 2;
        heads *= 2;
    }

    large->cylinder = cylinders;
    large->head = heads;

    return 0;
}

int g_chs2idechs(tCHS chs, tIDECHS *idechs)
{
    tLBA lba;
    g_chs2lba(chs, &lba);
    g_lba2idechs(lba, idechs);

    return 0;
}

//изучить и исправить
int g_large2chs(tLARGE large, tCHS *chs)
{
    int cylinders = large.cylinder;
    int heads = large.head + 1;

    chs->sector = large.sector;

    while (heads/2 > 0)
    {
        cylinders *= 2;
        heads /= 2;
    }

    if (cylinders < 1023 && heads < 15)
    {
        chs->cylinder = cylinders;
        chs->head = heads - 1;
    }

    return 0;
}

//изучить и исправить
int g_large2idechs(tLARGE large, tIDECHS *idechs)
{
    tLBA lba;
    g_large2lba(large, &lba);
    g_lba2idechs(lba, idechs);

    return 0;
}

int g_idechs2chs (tIDECHS idechs, tCHS *chs)
{
    tLBA lba;
    g_idechs2lba(idechs, &lba);
    g_lba2chs(lba, chs);

    return 0;
}

int g_chs2lba(tCHS chs, tLBA *lba)
{
    *lba = chs.cylinder * chs.head * chs.sector;

    return 0;
}

//изучить и исправить
int g_large2lba(tLARGE large, tLBA *lba)
{
    *lba = large.cylinder * large.head * large.sector;

    return 0;
}

int g_idechs2lba(tIDECHS idechs, tLBA *lba)
{
    *lba = idechs.cylinder * idechs.head * idechs.sector;

    return 0;
}

/* adresses */
int a_lba2chs (tCHS geometry, tLBA lba, tCHS *chs)
{
    int temp1 = lba/geometry.sector;
    int temp2 = lba%geometry.sector;

    chs->head = temp1%geometry.head;
    chs->cylinder = temp1/geometry.head;
    chs->sector = temp2 + 1;

    return 0;
}

int a_lba2large (tLARGE geometry, tLBA lba, tLARGE *large)
{


    return 0;
}

int a_lba2idechs (tIDECHS geometry, tLBA lba, tIDECHS *idechs)
{


    return 0;
}

int a_chs2lba (tCHS geometry, tCHS chs, tLBA *lba)
{
    *lba = (chs.cylinder*geometry.head + chs.head)*geometry.sector + chs.sector - 1;

    return 0;
}

int a_large2lba (tLARGE geometry, tLARGE large, tLBA *lba)
{


    return 0;
}

int a_idechs2lba (tIDECHS geometry, tIDECHS idechs, tLBA *lba)
{


    return 0;
}

int a_large2chs (tLARGE geometry1, tCHS geometry2, tLARGE large, tCHS *chs)
{


    return 0;
}

int a_large2idechs (tLARGE geometry1, tIDECHS geometry2, tLARGE large, tIDECHS *idechs)
{


    return 0;
}

int a_chs2large (tCHS geometry1, tLARGE geometry2, tCHS chs, tLARGE *large)
{


    return 0;
}

int a_idechs2large (tIDECHS geometry1, tLARGE geometry2, tIDECHS idechs, tLARGE *large)
{


    return 0;
}

int a_chs2idechs (tCHS geometry1, tIDECHS geometry2, tCHS chs, tIDECHS *idechs)
{


    return 0;
}

int a_idechs2chs (tIDECHS geometry1, tCHS geometry2, tIDECHS idechs, tCHS *chs)
{


    return 0;
}
