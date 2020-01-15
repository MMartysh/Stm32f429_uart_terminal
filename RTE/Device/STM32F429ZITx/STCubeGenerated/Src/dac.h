#ifndef DAC_H
#define DAC_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

void MX_DAC_Init(void);
HAL_StatusTypeDef startDAC(void);
HAL_StatusTypeDef stopDAC(void);
HAL_StatusTypeDef setValue(float valVolt);
uint32_t getValue(void);

#endif
