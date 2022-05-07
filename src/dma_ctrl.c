#include "stm32f4xx_hal.h"
#include "dma_ctrl.h"

//dma handler uart_rx

DMA_HandleTypeDef hdma_usart1_rx;

//dma handler uart_tx

DMA_HandleTypeDef hdma_usart1_tx;

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes DMA channels

 @param         None.

 @return        None.
*/
/* ----------------------------------------------------------------------------
 */
void dmaInit(void) 
{
  __HAL_RCC_DMA2_CLK_ENABLE();
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Set up DMA channel for UART1 RX

 @param         None.

 @return        None.
*/
/* ----------------------------------------------------------------------------
 */
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Set up DMA channel for UART1 TX

 @param         None.

 @return        None.
*/
/* ----------------------------------------------------------------------------
 */
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}
