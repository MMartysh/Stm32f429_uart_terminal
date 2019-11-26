#ifndef USART_IO_H
#define USART_IO_H
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "stm32f4xx_hal_conf.h"         // Keil::Device:STM32Cube Framework:Classic

void uart_dma_init(USART_HandleTypeDef* huart2);
void uart_gpio_init(void);
void uart_init(USART_HandleTypeDef* huart2);
HAL_StatusTypeDef uart_send(uint8_t* data, uint16_t size);
HAL_StatusTypeDef uart_receive(uint8_t* data, uint16_t size);


#endif
