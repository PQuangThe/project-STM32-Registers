#include "sys_clock.h"

__IO uint32_t Count_Tick;


//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//   RCC_OscInitStruct.PLL.PLLM = 4;
//   RCC_OscInitStruct.PLL.PLLN = 168;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//   RCC_OscInitStruct.PLL.PLLQ = 4;
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;


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

void Init_Tick_Tim(void){

    uint32_t PrescalerValue;
    uint32_t regtimcr;
    uint32_t prioritygroup = 0x00U;

    //Enable clock TIM2 
    RCC->APB1ENR |= (0x1UL << 0U);

    //calculate to have TIM2 counter clock equal to 1MHz
    PrescalerValue = (uint32_t)((((168000000 / 4) * 2U) / 1000000U) -1U);

    regtimcr = TIM2->CR1;

    //Direction and Center-aligned mode selection (upcounter mode)
    regtimcr &= ~((0x1UL << 4U)|(0x3UL << 5U));

    //Set the clock division (0)
    regtimcr &= ~(0x3UL << 8U);

    //Set the auto-reload preload (disable)
    regtimcr &= ~(0x1UL << 7U);

    TIM2->CR1 = regtimcr;

    //Set value auto-reload register
    TIM2->ARR = (uint32_t)999;
    
    //Set the Prescaler value (83)
    TIM2->PSC = PrescalerValue;

    //Re-initialize the counter and generates an update of the registers (UG)
    TIM2->EGR = 0x0001;

    //Clear update interrupt flag
    TIM2->SR &= ~(0x0001);

    //Enable the specified TIM Update interrupt
    TIM2->DIER |= (0x1UL);
    

    //Enable the TIM2 global Interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    
    prioritygroup = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(prioritygroup, 15, 0));
    
    //Counter enable
    TIM2->CR1  |= (0x1UL);

}

void Init_Tick(void){
    SysTick_Config(SystemCoreClock / 1000U);
}


void Inc_Tick(void){
    Count_Tick++;
}
uint32_t Get_Tick(void){
    return Count_Tick;
}

void s_delay(uint32_t ms){
    uint32_t _t =Get_Tick();
    while ((Get_Tick() - _t) < ms){
    }
}