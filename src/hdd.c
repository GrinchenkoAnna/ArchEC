#include "hdd.h"

int g_lba2chs(tLBA lba, tCHS *chs)
{
    chs->sector = 63;
    printf("lba = %d\n", lba);

    int L1 = lba/63;
    int L2 = L1/1023;
    printf("L1 = %d, L2 = %d\n", L1, L2);
    for (int i = 0; i < 8; i++)
    {
        if (L2 < LBA[i])
        {
            L2 = LBA[i];
            break;
        }
    }
    chs->head = L2;

    chs->cylinder = L1/L2;

    return 0;
}
