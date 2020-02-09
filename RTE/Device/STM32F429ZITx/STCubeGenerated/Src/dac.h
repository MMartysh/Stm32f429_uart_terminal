#ifndef DAC_H
#define DAC_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

/**
* Initialization dac peripheral according to the specified parameters
*/

void MX_DAC_Init(void);

/**
	*function which starts dac
	*@return {HAL_StatusTypeDef}
 */
 
HAL_StatusTypeDef DAC_Start(void);

/**
	*function which stops dac
	*@return {HAL_StatusTypeDef}
 */
 
HAL_StatusTypeDef DAC_Stop(void);

/**
  *function which sets dac value
	*@param {float} valVolt
	*return {HAL_StatusTypeDef}
 */
 
HAL_StatusTypeDef DAC_SetValue(float p_fl_ValVolt);

/**
	*function which returns dac value
	*return {uint32_t}
 */
 
uint32_t 					DAC_GetValue(void);

#endif
