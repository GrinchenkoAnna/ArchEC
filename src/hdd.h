#include <stdio.h>
#include <stdint.h>

typedef struct
{
  uint16_t cylinder;
  uint8_t head;
  uint8_t sector;
} tCHS;

typedef struct
{
  uint16_t cylinder;
  uint8_t head;
  uint8_t sector;
} tLARGE;

typedef struct
{
  uint16_t cylinder;
  uint8_t head;
  uint8_t sector;
} tIDECHS;

typedef uint32_t tLBA;

int LBA[8] = { 2, 4, 8, 16, 128, 255 };

int g_lba2chs (tLBA, tCHS*);
int g_lba2large (tLBA, tLARGE*);
int g_lba2idechs (tLBA, tIDECHS*);
int g_chs2large (tCHS, tLARGE*);
int g_chs2lba (tCHS, tLBA*);
int g_chs2idechs (tIDECHS, tLBA*);
int g_large2chs (tLARGE, tCHS*);
int g_large2idechs (tLARGE, tIDECHS*);
int g_large2lba (tLARGE, tLBA*);
int g_idechs2chs (tIDECHS, tCHS*);
int g_idechs2lagre (tIDECHS, tLARGE*);
int g_idechs2lba (tIDECHS, tLBA*);

int a_lba2chs (tCHS geometry, tLBA, tCHS*);
int a_lba2large (tLARGE geometry, tLBA, tLARGE*);
int a_lba2idechs (tIDECHS geometry, tLBA, tIDECHS*);
int a_chs2lba (tCHS geometry, tCHS, tLBA*);
int a_large2lba (tLARGE geometry, tLARGE, tLBA*);
int a_idechs2lba (tIDECHS geometry, tIDECHS, tLBA*);
int a_large2chs (tLARGE geometry1, tCHS geometry2, tLARGE, tCHS*);
int a_large2idechs (tLARGE geometry1, tIDECHS geometry2, tLARGE, tIDECHS*);
int a_chs2large (tCHS geometry1, tLARGE geometry2, tCHS, tLARGE*);
int a_idechs2large (tIDECHS geometry1, tLARGE geometry2, tIDECHS, tLARGE*);
int a_chs2idechs (tCHS geometry1, tIDECHS geometry2, tCHS, tIDECHS*);
int a_idechs2chs (tIDECHS geometry1, tCHS geometry2, tIDECHS, tCHS*);
