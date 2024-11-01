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



int __io_putchar(int ch) {
    Usart_Transmit((uint8_t *)&ch, 1, 1000);
    return ch;
}



void ProcessString(uint8_t *str,uint8_t *argc, uint8_t *argv[])
{
  uint8_t ct = 0;
  char *token = strtok((char *)str, " ");
  while (token != NULL) 
  {
    strncpy((char *)argv[ct], token, 10);
    //argv[ct][9] = '\0'; 
    ct++;
    token = strtok(NULL, " ");
  }
  *argc=ct;
}

/*

*/
void PerformW25q(uint8_t *pData,uint8_t argc, uint8_t *argv[])
{
  if (strcmp((char *)argv[0],"r0")==0)
  {
    uint32_t ch1 = (uint32_t)atol((char *)argv[1]);
    uint32_t ch2 = (uint32_t)atol((char *)argv[2]);

    printf("\nRead flash w25q address: %ld, len: %ld\n",ch1,ch2);
    W25Q_Read_Byte(ch1,pData,ch2,1000);
    for ( int i = 0; i < ch2; i++)
    {
      printf("%02X ",pData[i]);
    }
    printf("\n");
    //return 1;
  }else if (strcmp((char *)argv[0],"ri")==0)
  {
    W25Q_ReadID();
    //return 1;
  } else if(strcmp((char *)argv[0],"wp")==0)
  {
    if( argc>2)
    {
      uint32_t ch =(uint32_t)atol((char *)argv[1]);;
      uint8_t wdata[16];
      uint16_t i=0;
      while ( i < (argc-2) )
      {
        wdata[i] = (uint32_t)atol((char *)argv[i+2]);
        i++;
      }
      printf("\nDeleting the sector to be write\n");
      W25Q_Erase_Sector((uint16_t)(ch/16/256));
      printf("Programing...\n");
      W25Q_Program_Page(ch,wdata,i);
      printf("Programing successly\n");
      
    }
  }
  else
  {
    printf("\nWrong data\n");
  }
  //return 0;
}

void vTaskLED (void *pvParameters)
{
  uint32_t pa = (uint32_t)pvParameters;
  uint16_t LDelay_[2]={2000,500};
	while(1)
	{
    //printf("hello pham the day\n");
    gpio_tog(GPIOA,pa+6);
		vTaskDelay(LDelay_[pa]);
	}
}

void vTask1 (void *pvParameters)
{
  //char delim[] = " ";
  //char nl[] = "\r\n";
	while(1)
	{
    uint8_t *dynamicArray = (uint8_t *)pvPortMalloc(64 * sizeof(uint8_t));
    memset(dynamicArray, 0, 64 * sizeof(uint8_t));
    if (dynamicArray != NULL) 
    {
      if(GetFromBuff_DMA(dynamicArray))
      {
          uint8_t **tokens = (uint8_t **)pvPortMalloc(10 * sizeof(uint8_t*));
          for (int i = 0; i < 10; i++) 
          {
            tokens[i] = (uint8_t *)pvPortMalloc(10 * sizeof(uint8_t));
          }

          uint8_t token_count;
          ProcessString(dynamicArray,&token_count,tokens);
          uint8_t *Arr = (uint8_t *)pvPortMalloc(16 * sizeof(uint8_t));
          PerformW25q(Arr,token_count,tokens);
          vPortFree(Arr);

          for (int i = 0; i < 10; i++) {
            vPortFree(tokens[i]);
          }
          vPortFree(tokens);
      }
      vPortFree(dynamicArray);
    }
    
		vTaskDelay(100);
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
  Uart1_Receive_DMA();
  W25Q_Reset();
  

  xTaskCreate(vTaskLED, "Led_blink" , 64 , (void *)0, 0, NULL);
  xTaskCreate(vTaskLED, "Led_blink1", 64 , (void *)1, 1, NULL);
  xTaskCreate(vTask1  , "Task_1"    , 4096, NULL     , 3, NULL);

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
