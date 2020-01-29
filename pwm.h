#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

void 			HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void 			MX_TIM1_Init(void);
void 			PWM_Start(uint32_t p_ui_DutyCycle);
void 			PWM_Stop(void);
uint32_t	PWM_GetPulse(void);

#endif
