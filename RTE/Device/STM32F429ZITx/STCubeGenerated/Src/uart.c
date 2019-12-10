#include "main.h"

UART_HandleTypeDef huart1;


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

HAL_StatusTypeDef uartTransmit(uint8_t *data,uint16_t size, uint32_t timeout)
{
	return HAL_UART_Transmit(&huart1, data, size,timeout);
}
HAL_StatusTypeDef uartReceive(uint8_t *data,uint16_t size, uint32_t timeout)
{
	return HAL_UART_Receive(&huart1, data, size,timeout);
}


