#ifndef SPI_H
#define SPI_H
#include "stm32f4xx_hal.h"

void MX_SPI1_Init(void);
HAL_StatusTypeDef spiTransmit(uint8_t *data,uint16_t size, uint32_t timeout);
HAL_StatusTypeDef spiReceive(uint8_t *data,uint16_t size, uint32_t timeout);
#endif