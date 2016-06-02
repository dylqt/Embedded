#ifndef __STC15_FLASH_H__
#define __STC15_FLASH_H__

#include "iap15.h"
#include "intrins.h"
#include "type.h"


void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
BYTE sequential_write_flash_in_one_sector(WORD begin_addr, WORD counter, BYTE array[]);
BYTE write_flash_with_protect_in_one_sector(WORD begin_addr, WORD counter, BYTE array[]);

#endif
