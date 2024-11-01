#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "sys_clock.h"
#include <string.h>

#define _SP1_GET_FLAG(__FLAG__)               (((SPI1->SR) & (__FLAG__)) == (__FLAG__))

void SPI_Init(void);
void SPI1_EndRxTx(void);
void SPI1_Transmit(uint8_t *txData, uint16_t txsize, uint16_t timeout_);
void SPI1_Receive(uint8_t *rxData, uint16_t rxsize, uint16_t timeout_);
void SPI1_TransmitReceive(uint8_t *txData, uint8_t *rxData, uint16_t txsize, uint16_t rxsize, uint16_t timeout_);
uint8_t SPI1_Receive_Byte(uint8_t rxData);

#ifdef __cplusplus
}
#endif

#endif