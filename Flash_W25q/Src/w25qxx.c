#include "w25qxx.h"


#define numBLOCK 32  // number of total blocks for 16Mb flash

void W25Q_Delay(uint32_t time)
{
	s_delay(time);
}

void csLOW (void)
{
	gpio_write (GPIOB, 0, 0);
}

void csHIGH (void)
{
	gpio_write (GPIOB, 0, 1);
}

void SPI_Write (uint8_t *data, uint8_t len)
{
	SPI1_Transmit(data, len, 2000);
}

void SPI_Read (uint8_t *data, uint8_t len)
{
	SPI1_Receive(data, len, 5000);
}

/**************************************************************************************************/

void W25Q_Reset (void)
{
	uint8_t tData[2];
	tData[0] = 0x66;  // enable Reset
	tData[1] = 0x99;  // Reset
	csLOW();
	SPI_Write(tData, 2);
	csHIGH();
	W25Q_Delay(100);
}

void W25Q_ReadID (void)
{
	uint8_t tData = W25Q_READ_JEDEC;  // Read JEDEC ID
	//uint8_t tx[4] = {0x9F, 0xff, 0xff, 0xff};
	uint8_t rx[3];
	//uint8_t rData[3];
	csLOW();
	SPI_Write(&tData, 1);
	//SPI1_TransmitReceive(tx, rx, 4,4, 1000);
	SPI_Read(rx,3);

	csHIGH();
	printf("\nID: 0x%02X%02X%02X\r\n", rx[0], rx[1], rx[2]);
}

/*
*/
void W25Q_Read_Byte (uint32_t Address, uint8_t *pData, uint32_t size,uint32_t timeout_)
{
	uint8_t cmd[4];
	cmd[0]=W25Q_READ_DATA;
	cmd[1]= (uint8_t)((Address >> 16) & 0xFF);
	cmd[2]= (uint8_t)((Address >> 8) & 0xFF);
	cmd[3]= (uint8_t)( Address & 0xFF);
	csLOW();
	SPI1_Transmit(cmd,4,500);
	SPI1_Receive(pData,size,timeout_);
	csHIGH();
}

/*
	Enable write 
*/
void W25Q_Enable_Write(void)
{
	uint8_t data=W25Q_WRITE_ENABLE;
	csLOW();
	SPI_Write(&data,1);
	csHIGH();
	W25Q_Delay(5);
}

/*

*/
void W25Q_Disable_Write(void)
{
	uint8_t data=W25Q_WRITE_DISABLE;
	csLOW();
	SPI_Write(&data,1);
	csHIGH();
	W25Q_Delay(5);
}

/*
	Erase sectors
*/
void W25Q_Erase_Sector (uint16_t numsector)
{
	uint8_t tData[6];
	uint32_t memAddr = numsector*16*256;   // Each sector contains 16 pages * 256 bytes

	W25Q_Enable_Write();
	tData[0] = W25Q_SECTOR_ERASE;  // Erase sector

	tData[1] = (memAddr>>16)&0xFF;  // MSB of the memory Address
	tData[2] = (memAddr>>8)&0xFF;
	tData[3] = (memAddr)&0xFF; // LSB of the memory Address

	csLOW();
	SPI_Write(tData, 4);
	csHIGH();

	W25Q_Delay(500);  // 450ms delay for sector erase

	W25Q_Disable_Write();

}
void W25Q_Write_Byte (uint32_t Address, uint8_t *pData, uint32_t size)
{
	uint8_t cmd[6];
	//uint32_t memAddr = numsector*16*256;   // Each sector contains 16 pages * 256 bytes

	W25Q_Enable_Write();
    cmd[0] = W25Q_PAGE_PROGRAM; // Page Program command
    cmd[1] = (Address >> 16) & 0xFF;
    cmd[2] = (Address >> 8) & 0xFF;
    cmd[3] = Address & 0xFF;

	csLOW();
	SPI_Write(cmd, 4);
	SPI_Write(pData,size);

	csHIGH();

	W25Q_Delay(5);  

	W25Q_Disable_Write();
}

void W25Q_Program_Page (uint32_t Address, uint8_t *pData, uint32_t size)
{
	uint32_t numAddress=Address;

	uint32_t quota;
	uint32_t offset_page;
	quota=0;

	// define the starting write position inside the first Flash page to write...
	offset_page=Address & 0xff;
	while ((size-quota+offset_page)>256)
	{
		W25Q_Write_Byte(numAddress+quota,pData+quota,256-offset_page);
		quota+=(256-offset_page);
		offset_page=0;
	}
	if ((size-quota)>0) {
		W25Q_Write_Byte(numAddress+quota,pData+quota,size-quota);
	}

}