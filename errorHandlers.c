#include "stm32f4xx_hal.h"
#include "Board_LED.h"
#include "errorHandlers.h"

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Handles error states

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void Error_Handler(void)
{
	/**
	* function which initializes leds
	*/
	
	LED_Initialize();
	
	/**
	* turn on led
	*/
	
	LED_On(0);
	while(1)
	{
	}
}
