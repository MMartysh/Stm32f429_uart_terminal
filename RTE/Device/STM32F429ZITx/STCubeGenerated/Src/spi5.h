#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

#define SPI_TIMEOUT 50
#define SPI_READ 0x80

void 		MX_SPI5_Init(void);
uint8_t SPI5_Read(uint8_t p_ui_Address);
void 		SPI5_Write(uint8_t p_ui_Address, uint8_t p_ui_Data);
#endif
