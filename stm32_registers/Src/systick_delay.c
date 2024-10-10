#include "systick_delay.h"

__IO uint32_t Count_Tick;

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
