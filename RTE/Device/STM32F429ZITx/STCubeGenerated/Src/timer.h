#ifndef TIMER_H
#define TIMER_H
#include "stm32f4xx_hal.h"

void MX_TIM3_Init(void);
HAL_StatusTypeDef startTimer(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
float getTime(void);
#endif
