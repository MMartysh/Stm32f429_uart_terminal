#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "spi_ctrl.h"

//spi handler
SPI_HandleTypeDef hspi5;

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Writes 1 byte of data via SPI5

 @param[in]     p_ui_Address register address to write to.
 @param[in]     p_ui_Data data to be written.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void spiWriteByte(uint8_t p_ui_Address, uint8_t p_ui_Data)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
  HAL_SPI_Transmit(&hspi5, &p_ui_Address, 1, SPI_TIMEOUT);
  HAL_SPI_Transmit(&hspi5, &p_ui_Data, 1, SPI_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
} 


 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Reads 1 byte of data via SPI5

 @param[in]     p_ui_Address register address to read from.

 @return        One byte of data from given register. 
*/
/* ----------------------------------------------------------------------------
 */
uint8_t spiReadByte(uint8_t p_ui_Address)
{
	uint8_t data;
	p_ui_Address = p_ui_Address | SPI_READ;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
	HAL_SPI_Transmit(&hspi5,&p_ui_Address,1,SPI_TIMEOUT);
	HAL_SPI_Receive(&hspi5,&data,1,SPI_TIMEOUT);
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
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
}


