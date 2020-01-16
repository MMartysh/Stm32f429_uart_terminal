#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void MX_TIM1_Init(uint32_t pwm_pulse, uint32_t channel);
void startPWM(uint32_t channel);
void stopPWM(void);
uint32_t getPulse(uint32_t channel);

#endif
