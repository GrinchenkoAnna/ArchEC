#include <stdio.h>
#include <stdint.h>
#include <math.h>

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

#define CHS_HEAD 4
#define CHS_SECTOR 6

int LOG_H[8] = { 2, 4, 8, 16, 32, 64, 128, 255 };

int g_lba2chs (tLBA, tCHS*); //-
int g_lba2large (tLBA, tLARGE*); //-
int g_lba2idechs (tLBA, tIDECHS*); //-
int g_chs2large (tCHS, tLARGE*); //-
int g_chs2lba (tCHS, tLBA*);
int g_chs2idechs (tCHS, tIDECHS*); //-
int g_large2chs (tLARGE, tCHS*); //-
int g_large2idechs (tLARGE, tIDECHS*);
int g_large2lba (tLARGE, tLBA*);
int g_idechs2chs (tIDECHS, tCHS*); //-
int g_idechs2lagre (tIDECHS, tLARGE*);
int g_idechs2lba (tIDECHS, tLBA*);

int a_lba2chs (tCHS, tLBA, tCHS*);
int a_lba2large (tLARGE, tLBA, tLARGE*);
int a_lba2idechs (tIDECHS, tLBA, tIDECHS*);
int a_chs2lba (tCHS, tCHS, tLBA*);
int a_large2lba (tLARGE, tLARGE, tLBA*);
int a_idechs2lba (tIDECHS, tIDECHS, tLBA*);
int a_large2chs (tLARGE, tCHS, tLARGE, tCHS*);
int a_large2idechs (tLARGE, tIDECHS, tLARGE, tIDECHS*);
int a_chs2large (tCHS, tLARGE, tCHS, tLARGE*);
int a_idechs2large (tIDECHS, tLARGE, tIDECHS, tLARGE*);
int a_chs2idechs (tCHS, tIDECHS, tCHS, tIDECHS*);
int a_idechs2chs (tIDECHS, tCHS, tIDECHS, tCHS*);
