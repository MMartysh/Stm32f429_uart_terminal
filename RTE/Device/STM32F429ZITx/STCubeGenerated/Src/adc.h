#ifndef ADC_H
#define ADC_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

/**
* function which initializes adc peripheral according to the specified parameters
*/

void 			MX_ADC1_Init(void);

/**
 * function which starts adc
 */
 
void 			ADC_Start(void);

/**
 * function which stops adc
 */

void 			ADC_Stop(void);

/**
 *function which returns adc value
 *@param {uint32_t} time
 *@return {uint32_t}
 */
 
uint32_t 	ADC_GetValue(uint32_t p_ui_Time);

#endif
