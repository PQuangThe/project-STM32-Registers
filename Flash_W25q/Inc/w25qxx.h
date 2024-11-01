#ifndef __W25QXX_H__
#define __W25QXX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "sys_clock.h"
#include "gpio.h"
#include "spi.h"


#define    W25Q_WRITE_ENABLE             0x06
#define    W25Q_VOLATILE_SR_WE           0x50
#define    W25Q_WRITE_DISABLE            0x04
#define    W25Q_READ_DATA                0x03
#define    W25Q_FAST_READ                0x0B
#define    W25Q_PAGE_PROGRAM             0x02
#define    W25Q_SECTOR_ERASE             0x20
#define    W25Q_BLOCK_ERASE_32KB         0x52
#define    W25Q_BLOCK_ERASE_64KB         0xD8
#define    W25Q_CHIP_ERASE               0xC7
#define    W25Q_READ_JEDEC               0x9F

void W25Q_Reset (void);
void W25Q_ReadID (void);
void W25Q_Read_Byte (uint32_t Address, uint8_t *pData, uint32_t size,uint32_t timeout_);
void W25Q_Enable_Write(void);
void W25Q_Disable_Write(void);
void W25Q_Erase_Sector (uint16_t numsector);
void W25Q_Write_Byte (uint32_t Address, uint8_t *pData, uint32_t size);
void W25Q_Program_Page (uint32_t Address, uint8_t *pData, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif