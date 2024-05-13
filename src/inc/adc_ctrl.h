#ifndef ADC_CTRL_H
#define ADC_CTRL_H


void 			adcInit(void);
void 			adcStart(void);
void 			adcStop(void);
uint32_t 	adcGetValue(uint32_t timeout);

#endif
