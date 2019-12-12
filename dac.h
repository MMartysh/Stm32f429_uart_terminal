#ifndef DAC_H
#define DAC_H
#include "main.h"

void MX_DAC_Init(void);
HAL_StatusTypeDef startDAC(void);
HAL_StatusTypeDef stopDAC(void);
HAL_StatusTypeDef setValue(float valVolt);

#endif