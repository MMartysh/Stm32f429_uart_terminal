#ifndef ADC_H
#define ADC_H


void 			ADC1_Init(void);
void 			ADC_Start(void);
void 			ADC_Stop(void);
uint32_t 	ADC_GetValue(uint32_t p_ui_Time);

#endif
