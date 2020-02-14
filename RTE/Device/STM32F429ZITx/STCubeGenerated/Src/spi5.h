#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

#define SPI_TIMEOUT 50 /*Spi timeout duration*/
#define SPI_READ 0x80	/*Accelerometer address*/


/**
*Initialization spi peripheral according to the specified parameters
*/

void MX_SPI5_Init(void);

/**
*function which reads data from spi
*@param {uint8_t} p_ui_Address
*@return {uint8_t}
*/

uint8_t SPI5_Read(uint8_t p_ui_Address);

/**
*function which sends to spi
*@param {uint8_t} p_ui_Address
*@param {uint8_t} p_ui_Data
*/

void 		SPI5_Write(uint8_t p_ui_Address, uint8_t p_ui_Data);
#endif
