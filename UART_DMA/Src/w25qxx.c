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

void W25Q_ReadID (char *rData)
{
	//uint8_t tData = 0x9F;  // Read JEDEC ID
	uint8_t tx[4] = {0x9F, 0xff, 0xff, 0xff};
	uint8_t rx[4];
	//uint8_t rData[3];
	csLOW();
	//SPI_Write(&tData, 1);
	SPI1_TransmitReceive(tx, rx, 4,4, 1000);
	// SPI1_Receive_Byte(0x4B);
	// SPI1_Receive_Byte(0xFF);
	// SPI1_Receive_Byte(0xFF);
	// SPI1_Receive_Byte(0xFF);

	// rx[0]=SPI1_Receive_Byte(0x9F);
	// rx[1]=SPI1_Receive_Byte(0xFF);
	// rx[2]=SPI1_Receive_Byte(0xFF);
	// rx[3]=SPI1_Receive_Byte(0xFF);
	// rx[4]=SPI1_Receive_Byte(0x9F);
	// rx[5]=SPI1_Receive_Byte(0xFF);
	// rx[6]=SPI1_Receive_Byte(0xFF);
	// rx[7]=SPI1_Receive_Byte(0xFF);

	//SPI_Read(rx,3);

	csHIGH();
	sprintf(rData,"ID: 0x%02X%02X%02X%02X\r\n", rx[0], rx[1], rx[2], rx[3]);
	// sprintf(rData,"ID: 0x%02X%02X%02X%02X%02X%02X%02X%02X\r\n", rx[0], rx[1], rx[2], rx[3], rx[4], rx[5], rx[6], rx[7]);
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