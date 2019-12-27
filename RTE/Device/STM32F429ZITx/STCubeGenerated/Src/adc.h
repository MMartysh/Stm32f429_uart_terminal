#ifndef ADC_H
#define ADC_H
#include "stm32f4xx_hal.h"
void MX_ADC1_Init(void);
void startADC(void);
void stopADC(void);
uint32_t getValueADC(uint32_t time);
#endif
