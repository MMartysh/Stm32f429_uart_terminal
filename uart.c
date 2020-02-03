#include "uart.h"

//uart  handler

UART_HandleTypeDef huart1;


/**
 *Initializes the uart peripheral according to the specified parameters
 */
 
void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * function which sends data to uart
 */

HAL_StatusTypeDef UART_Transmit(uint8_t *p_pui_Data,uint16_t p_ui_Size, uint32_t p_ui_Timeout)
{
	return HAL_UART_Transmit(&huart1, p_pui_Data, p_ui_Size,p_ui_Timeout);
}

/**
 * function which gets data to uart
 */

HAL_StatusTypeDef UART_Receive(uint8_t *p_pui_Data,uint16_t p_ui_Size, uint32_t p_ui_Timeout)
{
	return HAL_UART_Receive(&huart1, p_pui_Data, p_ui_Size,p_ui_Timeout);
}


