#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "sys_clock.h"

#define _UART_ENABLE_IT(__INTERRUPT__)      (USART1->CR1 |= (__INTERRUPT__))
#define _UART_DISABLE_IT(__INTERRUPT__)     (USART1->CR1 &= ~(__INTERRUPT__))

#define _UART_START_RECEIVE_IT              _UART_ENABLE_IT( 0x1UL << 5U)
#define _UART_START_IDLE_IT                 _UART_ENABLE_IT( 0x1UL << 4U)

#define _CLEAR_IDLEFLAG                     \
  do{                                           \
    __IO uint32_t tmpreg = 0x00U;               \
    tmpreg = USART1->SR;                        \
    tmpreg = USART1->DR;                        \
    (void)tmpreg;                               \
  } while(0U) 



//Size buffer receive interrupt
#define BUFF_SIZE           64U
#define DMA_USART_TX        0
#define DMA_USART_RX        1 
#define TIMEOUT_DMA         1000

typedef struct
{
    uint16_t head;
    uint16_t end;
    uint8_t reload;
}PosData;




void USART1_Init(void);

void Usart_Send_Char(uint8_t chr);
void Usart_Send_String(uint8_t *s);

void Usart_Transmit(uint8_t *s, uint32_t size, uint32_t timeout);

void ReceiveToBuff(uint8_t buf);
uint8_t GetFromBuff(uint8_t* buf);

uint16_t ProcessNewData(uint8_t *buffer, uint16_t startbuf,uint16_t startPos, uint16_t size);
void HandleDMAData(uint8_t *buffer, uint16_t oldPos, uint16_t currentPos, uint16_t bufferSize);
uint8_t GetFromBuff_DMA(uint8_t* buf);

void Uart1_Transmit_DMA(uint8_t* Data, uint8_t LenData);

void Uart1_Cancel_DMA2_Stream2(void);

void Uart1_Receive_DMA(void);


void UART1_IRQHandlerRxCallback(void);
__WEAK void UART1_DMA2_Tx_IRQHandler(void);
void UART1_DMA2_RX_IRQHandler(void);

__WEAK void TranCompleteITCallback(void);

#ifdef __cplusplus
}
#endif

#endif