#include "errorHandlers.h"

/*
*function which initializes Error_Handler
*turn on red led, programme has error
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
