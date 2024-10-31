#include "spi.h"

void SPI_Init(void)
{
    SPI1->CR1 &= ~(0x1UL < 6U);

    //CPAL first 
    SPI1->CR1 &= ~(0x1UL << 0U);
    //CPOL LOW
    SPI1->CR1 &= ~(0x1UL << 1U);
    //Master configuration
    SPI1->CR1 |= (0x1UL << 2U);
    //Baud rate control
    SPI1->CR1  = ((SPI1->CR1 & ~(0x7UL << 3U)) | (0x2UL << 3U));
    //MSB FIRST
    SPI1->CR1 &= ~(0x1UL << 7U);
    //SSM
    SPI1->CR1 |= (0x1UL << 9U);
    //SSI 
    SPI1->CR1 |= (0x1UL << 8U);
    //8-bit data frame format
    SPI1->CR1 &= ~(0x1UL << 7U);
    //Enable SPI1 
    SPI1->CR1 |= (0x1UL << 6U);
}

/*

*/
// void SPI1_EndRxTx(void)
// {
//     while ()
// }
/*


*/
void SPI1_Transmit(uint8_t *txData, uint16_t txsize, uint16_t timeout_)
{
    volatile uint8_t pRxBuffPtr[txsize];
    SPI1_TransmitReceive(txData,(uint8_t*)pRxBuffPtr, txsize, txsize,timeout_);
}

/*

*/
void SPI1_Receive(uint8_t *rxData, uint16_t rxsize, uint16_t timeout_)
{
    uint8_t pTxBuffPtr[rxsize];
    memset(pTxBuffPtr, 0xff, rxsize * sizeof(uint8_t));

   SPI1_TransmitReceive(pTxBuffPtr,rxData, rxsize, rxsize,timeout_);
}

/*

*/
void SPI1_TransmitReceive(uint8_t *txData, uint8_t *rxData, uint16_t txsize, uint16_t rxsize, uint16_t timeout_)
{
    uint32_t ticks_=Get_Tick();
    uint8_t *pTxBuffPtr = txData;
    uint16_t TxCount    = txsize;
    uint8_t *pRxBuffPtr = rxData;
    //*pRxBuffPtr = SPI1->DR;
    uint16_t RxCount    = rxsize;
    uint8_t txallowed   = 1U;

    while ( (TxCount>0)||(RxCount>0) )
    {
        if ((_SP1_GET_FLAG(0x1UL << 1U)) && (TxCount>0) && (txallowed==1))
        {
            SPI1->DR = *pTxBuffPtr;
            pTxBuffPtr++;
            TxCount--;
            txallowed=0U;
        }
        if ((_SP1_GET_FLAG(0x1UL << 0U)) && (RxCount>0))
        {
            (* (uint8_t *)pRxBuffPtr)= *(uint8_t *)&SPI1->DR;
            pRxBuffPtr++;
            RxCount--;
            txallowed=1U;
        }
        
        if ((Get_Tick() - ticks_) >=  timeout_)
        {
            break;
        }
    }
}

uint8_t SPI1_Receive_Byte(uint8_t rxData)
{
    uint8_t bf=SPI1->DR;
    while ((_SP1_GET_FLAG(1<<1))==0);
    SPI1->DR = rxData;
    while ((_SP1_GET_FLAG(1<<0))==0);
    bf= SPI1->DR;    
    return bf;
}