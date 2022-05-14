#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "board.h"

static const gpioPinCfg gpioCfgArray[] =
{
    {GPIOA, {GPIO_PIN_0,  GPIO_MODE_ANALOG,     GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},                        //ADC_CH0
    {GPIOA, {GPIO_PIN_4,  GPIO_MODE_ANALOG,     GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},                        //DAC_CH1
    {GPIOA, {GPIO_PIN_6,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_LOW,       GPIO_AF2_TIM3}},    //DAC_CH1
    {GPIOA, {GPIO_PIN_9,  GPIO_MODE_AF_PP,      GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1}},  //UART1_TX
    {GPIOA, {GPIO_PIN_10, GPIO_MODE_AF_PP,      GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1}},  //UART1_RX
    {GPIOB, {GPIO_PIN_8,  GPIO_MODE_AF_OD,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH,      GPIO_AF4_I2C1}},    //I2C1_SCL
    {GPIOB, {GPIO_PIN_9,  GPIO_MODE_AF_OD,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH,      GPIO_AF4_I2C1}},    //I2C1_SDA
    {GPIOE, {GPIO_PIN_9,  GPIO_MODE_AF_PP,      GPIO_PULLUP, GPIO_SPEED_FREQ_LOW,       GPIO_AF1_TIM1}},    //PWM1_Ch1                                  
    {GPIOC, {GPIO_PIN_1,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_LOW}},                         //SPI5_CS
    {GPIOF, {GPIO_PIN_7,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH,      GPIO_AF5_SPI5}},    //SPI5_SCK
    {GPIOF, {GPIO_PIN_8,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH,      GPIO_AF5_SPI5}},    //SPI5_MISO
    {GPIOF, {GPIO_PIN_9,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH,      GPIO_AF5_SPI5}},    //SPI5_MOSI
    {GPIOG, {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP,  GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},                        //GREEN_LED
    {GPIOG, {GPIO_PIN_14, GPIO_MODE_OUTPUT_PP,  GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},                        //RED_LED
};

void boardInitPins(void)
{
    for(uint8_t i = 0; i < sizeof(gpioCfgArray) / sizeof(gpioCfgArray[0]); i++)
    {
        GPIO_InitTypeDef pin = gpioCfgArray[i].gpioPinHandle;
        HAL_GPIO_Init(gpioCfgArray[i].gpioPortHandle, &pin);
    }
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
}

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{  
    if(hadc->Instance==ADC1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();
    }
}
/**
* @brief DAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    if(hdac->Instance==DAC)
    {
        /* Peripheral clock enable */
        __HAL_RCC_DAC_CLK_ENABLE();
    }
}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    if(hspi->Instance==SPI5)
    {
        /* Peripheral clock enable */
        __HAL_RCC_SPI5_CLK_ENABLE();
    }
}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
    else if(htim_base->Instance==TIM3)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==USART1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();
    }
}
