#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void MX_TIM1_Init();
uint32_t startPWM(uint32_t dutyCycle);
void stopPWM();

#endif
