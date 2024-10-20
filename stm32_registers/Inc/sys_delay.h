#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f407xx.h"

#define USE_TIM_TICK_DEF    1

void Init_Tick_Tim(void);

void Init_Tick(void);

void Inc_Tick(void);

uint32_t Get_Tick(void);

void s_delay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif