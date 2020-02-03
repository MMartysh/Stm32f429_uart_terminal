#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

/**
 * Initializes tim1 to configure the peripheral instances according to the user configuration
 */
 
void 			HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/**
 *Initializes tim1 and pwm
 */
 
void 			MX_TIM1_Init(void);

/**
 * function which starts pwm
	*@param {uint32_t} dutyCycle
 */
 
void 			PWM_Start(uint32_t p_ui_DutyCycle);

/**
 * function which stops pwm
 */
 
void 			PWM_Stop(void);

#endif
