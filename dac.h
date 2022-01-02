#ifndef DAC_H
#define DAC_H

void DAC_Init(void);
HAL_StatusTypeDef DAC_Start(void);
HAL_StatusTypeDef DAC_Stop(void);
HAL_StatusTypeDef DAC_SetValue(float p_fl_ValVolt);
uint32_t 					DAC_GetValue(void);

#endif
