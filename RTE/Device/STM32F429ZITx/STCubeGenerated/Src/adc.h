#ifndef ADC_H
#define ADC_H
#include "main.h"
void MX_ADC1_Init(void);
void startADC(void);
void stopADC(void);
uint32_t getValueADC(uint32_t time);
#endif