/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"



void vTaskLED (void *pvParameters)
{
  uint32_t pa = (uint32_t)pvParameters;
  uint16_t LDelay_[2]={2000,500};
	while(1)
	{
    gpio_tog(GPIOA,pa+6);
		vTaskDelay(LDelay_[pa]);
	}
}

void vTask1 (void *pvParameters)
{
	while(1)
	{
    uint8_t *dynamicArray = (uint8_t *)pvPortMalloc(24 * sizeof(uint8_t));
    memset(dynamicArray, 0, 24 * sizeof(uint8_t));
    if (dynamicArray != NULL) {
      GetFromBuff_DMA(dynamicArray);
      Usart_Send_Char('\n');
      Usart_Send_String(dynamicArray);
      Usart_Send_Char('\n');
      vPortFree(dynamicArray);
    }
    
		vTaskDelay(100);
	}
}
/*

*/
void vTaskFlash_W25(void *pvParameters)
{
  uint32_t Address=0;
  while(1)
	{
    char *Array = (char *)pvPortMalloc(40 * sizeof(char));
    uint8_t *rx = (uint8_t *)pvPortMalloc(16 * sizeof(uint8_t));
    // if (Array != NULL) {
    //   W25Q_ReadID((char *)Array);
    //   Usart_Transmit(Array,strlen((char *)Array),1000);
    //   vPortFree(Array);
    // }
    W25Q_Read_Byte(Address,rx,16,1000);

    sprintf(Array," 0x%02X%02X%02X%02X \r\n", rx[0], rx[1], rx[2], rx[3]);
    Usart_Transmit((uint8_t *)Array,strlen((char *)Array),1000);
    Address += 16;
    vPortFree(Array);
    vPortFree(rx);
		vTaskDelay(2000);
	}
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  USART1_Init();
  SPI_Init();
  //Uart1_Receive_DMA();
  W25Q_Reset();

  xTaskCreate(vTaskLED, "Led_blink" , 64 , (void *)0, 0, NULL);
  xTaskCreate(vTaskLED, "Led_blink1", 64 , (void *)1, 1, NULL);
  //xTaskCreate(vTask1  , "Task_1"    , 1024, NULL     , 3, NULL);

  xTaskCreate(vTaskFlash_W25,"Task_flash_w25", 2048 , NULL, 2, NULL);
	// Start the Scheduler
	vTaskStartScheduler();
  

  while (1)
  {

  }

  return 1;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
