#include "spi5.h"

SPI_HandleTypeDef hspi5;
void writeSPI(uint8_t address, uint8_t data)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
  HAL_SPI_Transmit(&hspi5,&address,1,SPI_TIMEOUT);
  HAL_SPI_Transmit(&hspi5,&data,1,SPI_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
} 
void readSPI(uint8_t address, uint8_t data)
{
	address = address | SPI_READ;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //CS --> Low
	HAL_SPI_Transmit(&hspi5,&address,1,SPI_TIMEOUT);
	HAL_SPI_Receive(&hspi5,&data,1,SPI_TIMEOUT);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); //CS --> High
}

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
	writeSPI(L3GD20_CTRL_REG1_ADDR,0x0F);
}

float L3GD20_GetAngularRateX(float sensitivity)
{
	float ang;
	uint8_t x_h;
	uint8_t x_l;
	int16_t xdata_raw;

	readSPI(L3GD20_OUT_X_L_ADDR,x_l);
	readSPI(L3GD20_OUT_X_H_ADDR,x_h);
	
	xdata_raw = (int16_t)( (uint16_t)(x_h << 8) + x_l );
	ang = (float)xdata_raw/sensitivity;
	return ang;
}

float L3GD20_GetAngularRateY(float sensitivity)
{
	float ang;
	uint8_t y_h;
	uint8_t y_l;
	int16_t ydata_raw;

	readSPI(L3GD20_OUT_Y_L_ADDR,y_l);
	readSPI(L3GD20_OUT_Y_H_ADDR,y_h);
	
	ydata_raw = (int16_t)( (uint16_t)(y_h << 8) + y_l );
	ang = (float)ydata_raw/sensitivity;
	return ang;
}

float L3GD20_GetAngularRateZ(float sensitivity)
{
	float ang;
	uint8_t z_h;
	uint8_t z_l;
	int16_t zdata_raw;

	readSPI(L3GD20_OUT_Z_L_ADDR,z_l);
	readSPI(L3GD20_OUT_Z_H_ADDR,z_h);
	
	zdata_raw = (int16_t)( (uint16_t)(z_h << 8) + z_l );
	ang = (float)zdata_raw/sensitivity;
	return ang;
}

