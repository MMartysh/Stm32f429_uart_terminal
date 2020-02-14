#include "spi5.h"

//spi handler

SPI_HandleTypeDef hspi5;

/**
*function which reads data from spi
*@param {uint8_t} p_ui_Address
*@return {uint8_t}
*/

void SPI5_Write(uint8_t p_ui_Address, uint8_t p_ui_Data)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
  HAL_SPI_Transmit(&hspi5,&p_ui_Address,1,SPI_TIMEOUT);
  HAL_SPI_Transmit(&hspi5,&p_ui_Data,1,SPI_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
} 


/**
*function which reads data from spi
*@param {uint8_t} p_ui_Address
*@return {uint8_t}
*/

uint8_t SPI5_Read(uint8_t p_ui_Address)
{
	uint8_t data;
	p_ui_Address = p_ui_Address | SPI_READ;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
	HAL_SPI_Transmit(&hspi5,&p_ui_Address,1,SPI_TIMEOUT);
	HAL_SPI_Receive(&hspi5,&data,1,SPI_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
	return data;
}

/**
*Initialization spi peripheral according to the specified parameters
*/

void MX_SPI5_Init(void)
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


