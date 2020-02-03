#include "dma.h"

//dma handler uart_rx

DMA_HandleTypeDef hdma_usart1_rx;

//dma handler uart_tx

DMA_HandleTypeDef hdma_usart1_tx;

/**
 *Initializes the dma peripheral according to the specified parameters
 */
 
void MX_DMA_Init(void) 
{
  __HAL_RCC_DMA2_CLK_ENABLE();
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

/**
 * function which reads data from uart
*/

void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

/**
 * function which sends data from uart
*/

void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}
