#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"
#include "errorHandlers.h"

void 							MX_USART1_UART_Init(void);
HAL_StatusTypeDef UART_Transmit(uint8_t *p_pui_Data,uint16_t p_ui_Size, uint32_t p_ui_Timeout);
HAL_StatusTypeDef UART_Receive(uint8_t *p_pui_Data,uint16_t p_ui_Size, uint32_t p_ui_Timeout);

#endif

