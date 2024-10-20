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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"



void SystemClock_Config(void);

//Task 1
void vTask1 (void *pvParameters)
{
  uint32_t reg_o;
	while(1)
	{
    reg_o = GPIOA->ODR;

		GPIOA->BSRR = ((reg_o & (0x1UL << 6U)) << 16U) | (~reg_o & (0x1UL << 6U));
		vTaskDelay(2000);
	}
}

//task 2
void vTask2 (void *pvParameters)
{
	uint32_t reg_b;
	while(1)
	{
    reg_b = GPIOA->ODR;

		GPIOA->BSRR = ((reg_b & (0x1UL << 7U)) << 16U) | (~reg_b & (0x1UL << 7U));
		vTaskDelay(3000);
	}
}
/* USER CODE END 0 */

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



  xTaskCreate(vTask1, "Task_1", 256, NULL, 1, NULL);
	xTaskCreate(vTask2, "Task_2", 256, NULL, 2, NULL);
	// Start the Scheduler
	vTaskStartScheduler();
  

  while (1)
  {
    // GPIOA->ODR &= ~((1UL << 6U)|(1UL << 7U));
    // GPIOA->ODR |=  (1UL << 7U);
    // s_delay(3000);
    // GPIOA->ODR &= ~((1UL << 6U)|(1UL << 7U));
    // GPIOA->ODR |=  (1UL << 6U);
    // s_delay(3000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
// __weak void SystemClock_Config(void)
// {
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//   /** Configure the main internal regulator output voltage
//   */
//   __HAL_RCC_PWR_CLK_ENABLE();
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

//   /** Initializes the RCC Oscillators according to the specified parameters
//   * in the RCC_OscInitTypeDef structure.
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//   RCC_OscInitStruct.PLL.PLLM = 4;
//   RCC_OscInitStruct.PLL.PLLN = 168;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//   RCC_OscInitStruct.PLL.PLLQ = 4;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /** Initializes the CPU, AHB and APB buses clocks
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }

/* USER CODE BEGIN 4 */
void SystemClock_Config(void){

  /* Configure Flash prefetch, Instruction cache, Data cache */ 
  FLASH->ACR |= (0x1UL << 9U) ;

  FLASH->ACR |= (0x1UL << 10U);

  FLASH->ACR |= (0x1UL << 8U);

  NVIC_SetPriorityGrouping(3U);

  RCC->APB2ENR |= (0x1UL << 14U);


  //Enable Power interface clock
  RCC->APB1ENR |= (1UL << 28);

  // Set regulator voltage scaling output is Scale 1 mode
  PWR->CR      |= (1UL << 14);

  //Enable HSE clock
  RCC->CR      |= (1UL << 16);

  //wait HSE ready
  while( !(RCC->CR & (1U << 17)));

  //disable PLL 
  RCC->CR     &= ~(1UL << 24);

  //Wait PLL disabled
  while( RCC->CR & (1UL << 25));

  /*Config PLL
    PLLN = 168
    PLLM = 4
    PLLP = 0 (Divide 2)
    PLLQ = 4
    PLL clock source is HSE

  */
  RCC->PLLCFGR = (0X00000000U  | (1UL << 22) | 4UL | (168UL << 6) | (4UL << 24) );

  //Enable PLL 
  RCC->CR     |= (1UL << 24);

  //Wait PLL ready
  while( !(RCC->CR & (1UL << 25)));

  //SET wait states to the LATENCY bits
  FLASH->ACR   = (FLASH->ACR & (~(0x00000007U))) | (5UL);

  //APB high-speed prescaler (APB2)  /2
  RCC->CFGR    = (RCC->CFGR & (~(0x0000E000U))) | (4UL << 13U);

  //SET APB Low speed prescaler APB1 /4
  RCC->CFGR    = (RCC->CFGR & (~(0x00001C00U))) | (5UL << 10U);

  //SET AHB prescaler
  RCC->CFGR    &= (~(0xFUL << 4));

  //set pll is SYSCLK source
  RCC->CFGR    = (RCC->CFGR & (~(0x00000003U))) | (2UL);

  SystemCoreClock = 168000000;
#if (USE_TIM_TICK_DEF == 1)
    Init_Tick_Tim();
#else
    Init_Tick();
#endif

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
