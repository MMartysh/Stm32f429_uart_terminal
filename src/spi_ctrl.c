#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "spi_ctrl.h"
#define SPI_TIMEOUT 50    /*Spi timeout duration*/
#define SPI_READ    0x80	/*Accelerometer address*/
//spi handler
SPI_HandleTypeDef spiHandler;

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Writes 1 byte of data via SPI5

 @param[in]     address register address to write to.
 @param[in]     data data to be written.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void spiWriteByte(uint8_t address, uint8_t data)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
    HAL_SPI_Transmit(&spiHandler, &address, 1, SPI_TIMEOUT);
    HAL_SPI_Transmit(&spiHandler, &data, 1, SPI_TIMEOUT);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
} 


 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Reads 1 byte of data via SPI5

 @param[in]     address register address to read from.

 @return        One byte of data from given register. 
*/
/* ----------------------------------------------------------------------------
 */
uint8_t spiReadByte(uint8_t address)
{
	uint8_t data;
	address = address | SPI_READ;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
	HAL_SPI_Transmit(&spiHandler, &address,1,SPI_TIMEOUT);
	HAL_SPI_Receive(&spiHandler, &data,1,SPI_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
	return data;
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes SPI5

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void spiInit(void)
{
    spiHandler.Instance = SPI5;
    spiHandler.Init.Mode = SPI_MODE_MASTER;
    spiHandler.Init.Direction = SPI_DIRECTION_2LINES;
    spiHandler.Init.DataSize = SPI_DATASIZE_8BIT;
    spiHandler.Init.CLKPolarity = SPI_POLARITY_LOW;
    spiHandler.Init.CLKPhase = SPI_PHASE_1EDGE;
    spiHandler.Init.NSS = SPI_NSS_SOFT;
    spiHandler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    spiHandler.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spiHandler.Init.TIMode = SPI_TIMODE_DISABLE;
    spiHandler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spiHandler.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&spiHandler) != HAL_OK)
    {
        Error_Handler();
    }
}


