#include "hdd.h"

/* Возможно, все неправильно */

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
    while (logic_heads / K > 16) { K *= 2; }

    chs->head = logic_heads/K;
    chs->cylinder = L1/chs->head;

    return 0;
}

int g_lba2large(tLBA lba, tLARGE *large)
{
    large->sector = 63;

    int L1 = lba/63;
    int logic_heads = L1/1024;
    for (int i = 0; i < 8; i++)
    {
        if (logic_heads < LOG_H[i])
        {
            logic_heads = LOG_H[i];
            break;
        }
    }

    int K = 2;
    while (logic_heads / K > 256) { K *= 2; }

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
    while (logic_heads / K > 16) { K *= 2; }

    idechs->head = logic_heads/K;
    idechs->cylinder = L1/idechs->head;

    return 0;
}

int g_chs2large(tCHS chs, tLARGE *large)
{
    int cylinders = chs.cylinder;
    int heads = chs.head;

    large->sector = chs.sector;

    while (cylinders/2 > 0 && heads*2 < 256)
    {
        cylinders /= 2;
        heads *= 2;
    }

    if (cylinders < 1024 && heads < 256)
    {
        large->cylinder = cylinders;
        large->head = heads;
    }

    return 0;
}

int g_chs2idechs(tCHS chs, tIDECHS *idechs)
{
    tLBA lba;
    g_chs2lba(chs, &lba);
    g_lba2idechs(lba, idechs);

    return 0;
}

int g_large2chs(tLARGE large, tCHS *chs)
{
    int cylinders = large.cylinder;
    int heads = large.head;

    chs->sector = large.sector;

    while (cylinders*2 < 1024 && heads/2 > 0)
    {
        cylinders *= 2;
        heads /= 2;
    }

    if (cylinders < 1024 && heads < 16)
    {
        chs->cylinder = cylinders;
        chs->head = heads;
    }

    return 0;
}

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

//переделать
int g_chs2lba (tCHS chs, tLBA *lba)
{
    int K = 2;
    int heads = 0;
    while (chs.head/K > 16) { K*= 2; }
    heads = chs.head/K;

    printf("heads = %d, cylinders = %d\n", heads, chs.cylinder*K);
    *lba = chs.cylinder*K * heads * chs.sector;

    return 0;
}

//переделать
int g_large2lba (tLARGE large, tLBA *lba)
{
    int K = 2;
    int heads = 0;
    while (large.head/K > 256) { K*= 2; }
    heads = large.head/K;

    printf("heads = %d, cylinders = %d\n", heads, large.cylinder*K);
    *lba = large.cylinder*K * heads * large.sector;

    return 0;
}

//переделать
int g_idechs2lba (tIDECHS idechs, tLBA *lba)
{
    int K = 2;
    int heads = 0;
    while (idechs.head/K > 16) { K*= 2; }
    heads = idechs.head/K;

    printf("heads = %d, cylinders = %d\n", heads, idechs.cylinder*K);
    *lba = idechs.cylinder*K * heads * idechs.sector;

    return 0;
}


