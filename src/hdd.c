#include "hdd.h"

int g_lba2chs(tLBA lba, tCHS *chs)
{
    chs->sector = 63;

    int L1 = lba/63;
    int logic_heads = L1/1023;
    for (int i = 0; i < 8; i++)
    {
        if (logic_heads < LBA[i])
        {
            logic_heads = LBA[i];
            break;
        }
    }

    int K = 2;
    while (logic_heads / K > 16) { K *= 2; }

    chs->head = logic_heads/K;
    chs->cylinder = L1/chs->head;

    return 0;
}

int g_chs2lba (tCHS chs, tLBA *lba)
{


    return 0;
}
