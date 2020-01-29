#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

void 							SysTick_Handler(void);
void 							MX_TIM3_Init(void);
HAL_StatusTypeDef TIMER_Start(void);
void 							HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
float 						TIMER_GetTime(void);

#endif
