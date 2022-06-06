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
    TIM_HandleTypeDef htim3;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 16000;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 15;
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
    Error_Handler();
    }
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
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);
}


