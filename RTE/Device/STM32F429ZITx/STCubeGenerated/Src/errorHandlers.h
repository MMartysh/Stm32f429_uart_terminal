#ifndef ERRORHANDLERS_H
#define ERRORHANDLERS_H
#include "stm32f4xx_hal.h"
#include "Board_LED.h"

/*
*function which initializes Error_Handler
*turn on red led, programme has error
*/

void Error_Handler(void);


#endif
