#ifndef UART_H
#define UART_H
#include "main.h"

void MX_USART1_UART_Init(void);
HAL_StatusTypeDef uartTransmit(uint8_t *data,uint16_t size, uint32_t timeout);
HAL_StatusTypeDef uartReceive(uint8_t *data,uint16_t size, uint32_t timeout);
#endif