#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "uart.h"

UART_HandleTypeDef huart1;


/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes UART with predefined parameters:
                      Baud rate - 115200
                      Wordlength - 8bit
                      Stopbits - 1
                      Parity - None

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void UART_Init(void)
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

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Transmits data via UART in polling blocking mode

 @param[in]     p_pui_Data pointer to a buffer to transmit data from.
 @param[in]     p_ui_Size transmit data size.
 @param[in]     p_ui_Timeout Operation timeout.

 @return        Status of the operation. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef UART_Transmit(uint8_t *p_pui_Data, uint16_t p_ui_Size, uint32_t p_ui_Timeout)
{
  HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, p_pui_Data, p_ui_Size,p_ui_Timeout);
	if(ret != HAL_OK && ret != HAL_TIMEOUT)
	{
		Error_Handler();
	}
	
		return ret;
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Receives the data via UART in polling blocking mode

 @param[out]    p_pui_Data pointer to store received data to.
 @param[in]     p_ui_Size received data size.
 @param[in]     p_ui_Timeout Operation timeout.

 @return        Status of the operation. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef UART_Receive(uint8_t *p_pui_Data, uint16_t p_ui_Size, uint32_t p_ui_Timeout)
{
  HAL_StatusTypeDef ret = HAL_UART_Receive(&huart1, p_pui_Data, p_ui_Size,p_ui_Timeout);
	if(ret != HAL_OK && ret != HAL_TIMEOUT)
	{
		Error_Handler();
	}
	
		return ret;
}

