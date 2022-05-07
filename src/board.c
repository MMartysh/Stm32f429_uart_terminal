#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "board.h"

static const gpioPinCfg gpioCfgArray[] =
{
  {GPIOA, {GPIO_PIN_0,  GPIO_MODE_ANALOG,     GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},  //ADC_CH0
  {GPIOA, {GPIO_PIN_4,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},  //DAC_CH1
  {GPIOA, {GPIO_PIN_9,  GPIO_MODE_AF_PP,      GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},  //UART1_TX
  {GPIOA, {GPIO_PIN_10, GPIO_MODE_AF_PP,      GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},  //UART1_RX
  {GPIOE, {GPIO_PIN_9,  GPIO_MODE_AF_PP,      GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},  //PWM1_Ch1                                  
  {GPIOC, {GPIO_PIN_1,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_LOW}},   //SPI5_CS
  {GPIOF, {GPIO_PIN_8,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},  //SPI5_SCK
  {GPIOF, {GPIO_PIN_8,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},  //SPI5_MISO
  {GPIOF, {GPIO_PIN_9,  GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},  //SPI5_MOSI
  {GPIOG, {GPIO_PIN_13, GPIO_MODE_OUTPUT_PP,  GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},  //GREEN_LED
  {GPIOG, {GPIO_PIN_14, GPIO_MODE_OUTPUT_PP,  GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH}},  //RED_LED
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
