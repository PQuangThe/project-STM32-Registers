#include "uart.h"
#include <stddef.h>

volatile uint8_t buff_rx_it[BUFF_SIZE];
#if(DMA_USART_RX == 1)
    volatile PosData _posdata={0,0,0};
#else
    volatile uint32_t rx_count=0U;
#endif
/*

    Configure the UART1

*/
void USART1_Init(void)
{
    uint32_t prioritygroup = 0x00U;
    uint32_t tickstart;
    //Enable the USART global Interrupt
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(prioritygroup, 5, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    //Disable the peripheral USART1
    USART1->CR1 &= ~(0x1UL << 13U);

    //Configure the UART Stop Bits: Set STOP[13:12] bits in CR2 (1 Stop bit)
    USART1->CR2 =((USART1->CR2 & ~(0x3UL << 12U)) | (0x0UL << 12U));

    //Oversampling mode (16)
    USART1->CR1 &= ~(0x1UL << 15U);

    //Word length (8)
    USART1->CR1 &= ~(0x1UL << 12U);

    //Parity control (none)
    USART1->CR1 &= ~(0x1UL << 10U);

    //Transmitter enable 
    USART1->CR1 |= (0x1UL << 3U);

    //Receiver enable 
    USART1->CR1 |= (0x1UL << 2U);
    
    //CTS disabled and RTS disabled
    USART1->CR3 &= ~((0x1UL << 9U) |(0x1UL << 8U));

    //Set Baud rate register (115200  at fAPB2 = 84 MHz)
    USART1->BRR = (uint32_t)0x2d9;
    //USART1->BRR =  (uint32_t)(0x88b8);

    /* In asynchronous mode, the following bits must be kept cleared:
     - LINEN and CLKEN bits in the USART_CR2 register,
     - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/

    USART1->CR2 &= ~((0x1UL << 11U) | (0x1UL << 14U));
    USART1->CR3 &= ~((0x1UL << 5U) | (0x1UL << 3U) | (0x1UL << 1U));

    //USART1->CR3 |= (0x1UL << 7U);
    USART1->CR3 |= (0x1UL << 6U);

#if(DMA_USART_TX == 1)
    uint32_t tmp = 0U;
    //enable DMA2 clock 
    RCC->AHB1ENR |= (0x1UL << 22U);

    NVIC_SetPriority(DMA2_Stream7_IRQn, NVIC_EncodePriority(prioritygroup, 5, 0));
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);

    //disable DMA
    DMA2_Stream7->CR &= ~(0x1UL);

    //wait EN=0
    tickstart = Get_Tick();
    while((DMA2_Stream7->CR & (0x1UL))!=0)
    {
        if((Get_Tick() - tickstart ) > TIMEOUT_DMA)
        {
            break;
        }
    }

    tmp = (uint32_t)(0x00000000U);
    /*
        CHSEL[2:0]: Channel selection        ( CHANNEL 4)
        PL[1:0]: Priority level              (LOW)

        MSIZE[1:0]: Memory data size         (BYTE)
        PSIZE[1:0]: Peripheral data size     (BYTE)
        MINC: Memory increment mode          (1)
        PINC: Peripheral increment mode      (0)
        DIR[1:0]: Data transfer direction    (Memory-to-peripheral)


    */
    tmp |= ((0x4UL << 25U) | (0x0UL << 16U) \
           |(0x0UL << 13U) | (0x0UL << 11U) \
           |(0x1UL << 10U) | (0x1UL << 6U));

    DMA2_Stream7->CR = tmp;

    //Clear all flag interrupt
    DMA2->HIFCR |= (0xFUL << 24U);

    //DMAT: DMA enable transmitter
    USART1->CR3 |= (0x1UL << 7U);

#endif
#if(DMA_USART_RX ==1)
    RCC->AHB1ENR |= (0x1UL << 22U);
    //disable DMA
    DMA2_Stream2->CR = 0; // Tắt DMA stream trước khi cấu hình
    //wait EN=0
    tickstart = Get_Tick();
    while((DMA2_Stream2->CR & (0x1UL))!=0)
    {
        if((Get_Tick() - tickstart ) > TIMEOUT_DMA)
        {
            break;
        }
    }
    // Cấu hình DMA stream cho RX
    DMA2_Stream2->CR |= (0x1UL << 10U) | (0x1UL << 8U); // Tăng địa chỉ bộ nhớ, circle mode
    DMA2_Stream2->CR |= (0x4UL << 25U); // Chọn kênh 4 (UART1 RX)
    DMA2_Stream2->CR |= (0x3UL << 16U); // Ưu tiên cao

    NVIC_SetPriority(DMA2_Stream2_IRQn, NVIC_EncodePriority(prioritygroup, 5, 0));
    NVIC_EnableIRQ(DMA2_Stream2_IRQn);

#endif

    //Enable the peripheral USART1
    USART1->CR1 |= (0x1UL << 13U);

}

/*
    Sends a char
*/
void Usart_Send_Char(uint8_t chr)
{
    while ( (USART1->SR & (0x1UL << 7U)) == (0x00000000U));

    USART1->DR = chr;

    while ( (USART1->SR & (0x1UL << 7U)) == (0x00000000U));

}

/*
    Sends an amount of data
*/
void Usart_Send_String(uint8_t *s)
{
    uint16_t count=0;
    while( s[count] != 0)
    {
        Usart_Send_Char(s[count]);
        count ++;
    }
}

void Usart_Transmit(uint8_t *sData, uint32_t sizedata, uint32_t timeout)
{
    uint32_t ticks_=Get_Tick();
    uint32_t _count = 0;
    while( _count < sizedata)
    {
        if((Get_Tick() - ticks_ ) > timeout)
        {
            break;
        }
        
        while ( (USART1->SR & (0x1UL << 7U)) == (0x00000000U))
        {
            if((Get_Tick() - ticks_ ) > timeout)
            {
                break;
            }
        }
        USART1->DR = sData[_count];
        _count++;
    }
}

#if(DMA_USART_RX == 0)
/*

*/
void ReceiveToBuff(uint8_t buf)
{
    buff_rx_it[rx_count] = buf;
    if(rx_count == BUFF_SIZE)
    {
        rx_count=0;
    }
    else
    {
        rx_count++;
    }
}

uint8_t GetFromBuff(uint8_t* buf)
{
    if(rx_count != 0U)
    {
        uint16_t i;
        for(i=0; (i<=rx_count) && (buff_rx_it[i] != 0);i++)
        {
            buf[i] = buff_rx_it[i];
        }
        rx_count=0U;
        return 1;
    }
    else
    {
        return 0;
    }
}
#else

    uint16_t ProcessNewData(uint8_t *buffer, uint16_t startbuf,uint16_t startPos, uint16_t size) 
    {
        for (uint16_t i = 0; i < size; startbuf++,i++) {
            // Xử lý dữ liệu tại buffer[(startPos + i) % BUFFER_SIZE]
            buffer[startbuf]=buff_rx_it[(startPos + i) % BUFF_SIZE];
        }
        return startbuf;
    }

    void HandleDMAData(uint8_t *buffer, uint16_t oldPos, uint16_t currentPos, uint16_t bufferSize) {
        if (currentPos >= oldPos) {
            // Dữ liệu mới nằm gọn trong một đoạn liên tiếp
            ProcessNewData(buffer,0, oldPos, currentPos - oldPos);
        } else {
            // Dữ liệu mới nằm cuối bộ đệm và tiếp tục ở đầu bộ đệmb
            uint16_t startbuf_count;
            startbuf_count =ProcessNewData(buffer,0, oldPos, bufferSize - oldPos);
            ProcessNewData(buffer,startbuf_count, 0, currentPos);
        }
        // Cập nhật vị trí con trỏ cũ
        
    }

    uint8_t GetFromBuff_DMA(uint8_t* buf)
    {
        uint16_t cdata= (uint16_t)DMA2_Stream2->NDTR;
        
        if(_posdata.end >= BUFF_SIZE)
        {
            _posdata.head=0;
            _posdata.end=(BUFF_SIZE-cdata);
        }
        else
        {
            _posdata.head=_posdata.end;
            _posdata.end=(BUFF_SIZE-cdata);
        }
        HandleDMAData(buf, _posdata.head, _posdata.end, BUFF_SIZE);
        if (_posdata.head == _posdata.end)
        {
            return 0;
        }
        return 1;
        
    }
#endif

/*

*/
void Uart1_Cancel_DMA2_Stream2(void)
{
    uint32_t tickstart=Get_Tick();
    //clear all interrupt dma
    DMA2_Stream2->CR  &= ~(0xfUL << 1U);
    DMA2_Stream2->FCR &= ~(0x1UL << 7U);
    //clear all flag interrupt dma
    DMA2->LIFCR |= (0x3fUL << 16U);

    //disable DMA
    DMA2_Stream2->CR &= ~(0x1UL);
    

    //wait EN=0
    while((DMA2_Stream2->CR & (0x1UL))!=0)
    {
        if((Get_Tick() - tickstart ) > TIMEOUT_DMA)
        {
            break;
        }
    }
}

/*

*/
void Uart1_Transmit_DMA(uint8_t* Data, uint8_t LenData)
{
    const uint32_t *tmp;
    tmp = (const uint32_t *)&Data;
    //
    DMA2_Stream7->NDTR = LenData;

    //
    DMA2_Stream7->PAR  = (uint32_t)(&USART1->DR);

    //
    DMA2_Stream7->M0AR = *(const uint32_t *)tmp;

    // Enanle Transfer complete interrupt and Half transfer interrupt
    DMA2_Stream7->CR  |= ((0x1UL << 4U) | (0x1UL << 3U));

    //  Enable dma
    DMA2_Stream7->CR  |= (0x1UL << 0U);

    USART1->SR  = ~(0x1UL << 6U);
}

/*

*/
void Uart1_Receive_DMA(void)
{
    DMA2_Stream2->PAR = (uint32_t)&USART1->DR; // Địa chỉ ngoại vi (UART DR)
    DMA2_Stream2->M0AR = (uint32_t)buff_rx_it; // Địa chỉ bộ nhớ (rxBuffer)
    DMA2_Stream2->NDTR = (uint32_t)BUFF_SIZE; // Kích thước dữ liệu

    DMA2_Stream2->CR  |= (0x1UL << 4U);
    DMA2_Stream2->CR  |= 0x01UL;
}
/*

*/
void UART1_IRQHandlerRxCallback(void)
{
    uint32_t srflags   = USART1->SR;
    uint32_t cr1reg    = USART1->CR1;
    //uint32_t cr3reg    = USART1->CR3;

    //interrupt is generated if RXNEIE=1 in the USART_CR1 register indicate received data is ready to be read
    if(((srflags & (0x1UL << 5U)) != 0U) && ((cr1reg & (0x1UL << 5U)) != 0U))
    {
#if (DMA_USART_RX ==0)
        ReceiveToBuff((uint8_t)USART1->DR);
#endif
    }

    if(((srflags & (0x1UL << 4U)) != 0U) && ((cr1reg & (0x1UL << 4U)) != 0U))
    {

    }
}

/*
    This function handles DMA2 Stream7 interrupt request.
*/
// void UART1_DMA2_Tx_IRQHandler(void)
// {
//     uint32_t srflags   = DMA2->HISR;

//     //tran complete interrupt
//     if((srflags & (0x1UL << 27U)) != 0U)
//     {
//         DMA2->HIFCR |= (0x1UL << 27U);
//         DMA2_Stream7->CR  &= ~((0x1UL << 4U) | (0x1UL << 3U));
//     }

//     //Half tran complete interrupt 
//     if((srflags & (0x1UL << 26U)) != 0U)
//     {
//         DMA2->HIFCR |= (0x1UL << 26U);
//     }
// }

// /*
//     This function handles DMA2 Stream2 interrupt request.
// */

__WEAK void TranCompleteITCallback(void)
{
        _posdata.reload=1;
}
__WEAK void UART1_DMA2_RX_IRQHandler(void)
{
    uint32_t srflags   = DMA2->LISR;

    //tran complete interrupt
    if((srflags & (0x1UL << 21U)) != 0U)
    {
        //DMA2_Stream2->CR  &= ~(0x1UL << 4U);
        DMA2->LIFCR |= (0x3FUL << 16U);
        TranCompleteITCallback();
    }

    //Half tran complete interrupt 
    if((srflags & (0x1UL << 20U)) != 0U)
    {
        DMA2->LIFCR |= (0x1UL << 20U);
    }
}