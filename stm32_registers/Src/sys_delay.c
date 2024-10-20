#include "sys_delay.h"

__IO uint32_t Count_Tick;


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