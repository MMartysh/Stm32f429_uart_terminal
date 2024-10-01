#ifndef DAC_CTRL_H
#define DAC_CTRL_H

void              dacInit(void);
HAL_StatusTypeDef dacStart(void);
HAL_StatusTypeDef dacStop(void);
HAL_StatusTypeDef dacSetValue(uint32_t voltage);
uint32_t 					dacGetValue(void);

#endif
