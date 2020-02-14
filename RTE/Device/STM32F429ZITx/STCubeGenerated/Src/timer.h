#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

/**
 *function which generates interrupt
*/

void SysTick_Handler(void);

/**
 * function which initializes tim3
 */
 
void MX_TIM3_Init(void);

/**
 *function which starts tim3
 *@return {HAL_StatusTypeDef}
 */
 
HAL_StatusTypeDef TIMER_Start(void);

/**
 *function which returns time from start programme
 *@return {float}
 */
 
float TIMER_GetTime(void);

#endif
