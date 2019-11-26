#include "usart_IO.h"

uint8_t dma_buffer[2000];
volatile uint8_t toggle = 0;


DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;
void uart_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();

  /**USART2 GPIO Configuration
  PA2     ------> USART2_TX
  PA3     ------> USART2_RX
  */
  /*GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/
}

void uart_dma_init(USART_HandleTypeDef* huart2)
{
  __DMA1_CLK_ENABLE();


  hdma_usart2_tx.Instance = DMA1_Stream6;
  hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_tx.Init.PeriphDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.Mode = DMA_NORMAL;
  hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_usart2_tx);
  __HAL_LINKDMA(huart2,hdmatx,hdma_usart2_tx);

	hdma_usart2_rx.Instance 					= DMA1_Stream5;
		hdma_usart2_rx.Init.Channel	 			= DMA_CHANNEL_4;
		hdma_usart2_rx.Init.Direction	        = DMA_PERIPH_TO_MEMORY;  
		hdma_usart2_rx.Init.PeriphInc    	    = DMA_PINC_DISABLE;
		hdma_usart2_rx.Init.MemInc				= DMA_MINC_ENABLE;
		hdma_usart2_rx.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;
		hdma_usart2_rx.Init.MemDataAlignment		= DMA_MDATAALIGN_BYTE;
		hdma_usart2_rx.Init.Mode 				= DMA_NORMAL;
		hdma_usart2_rx.Init.Priority				= DMA_PRIORITY_LOW;
		hdma_usart2_rx.Init.FIFOMode				= DMA_FIFOMODE_DISABLE;
		hdma_usart2_rx.Init.FIFOThreshold		= DMA_FIFO_THRESHOLD_FULL;
		hdma_usart2_rx.Init.MemBurst				= DMA_MBURST_SINGLE;
		hdma_usart2_rx.Init.PeriphBurst			= DMA_PBURST_SINGLE;
		
		HAL_DMA_Init(&hdma_usart2_rx);
		__HAL_LINKDMA(huart2, hdmarx, hdma_usart2_rx);
		

 /* HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	 HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);*/
}

void uart_init(USART_HandleTypeDef* huart2)
{
  __USART2_CLK_ENABLE();

  huart2->Instance = USART2;
  huart2->Init.BaudRate = 115200;
  huart2->Init.WordLength = USART_WORDLENGTH_8B;
  huart2->Init.StopBits = USART_STOPBITS_1;
  huart2->Init.Parity = USART_PARITY_NONE;
  huart2->Init.Mode = USART_MODE_TX_RX;
	huart2->hdmarx=&hdma_usart2_rx;
	huart2->hdmatx=&hdma_usart2_tx;
  HAL_USART_Init(huart2);
	

 /* HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);*/
}
HAL_StatusTypeDef uart_send(uint8_t* data, uint16_t size)
{
	//return HAL_USART_Transmit_DMA(&huart2,data,size);
}

/*void DMA1_Stream5_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
    HAL_DMA_IRQHandler(&hdma_usart2_rx);
}
void DMA1_Stream6_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA1_Stream6_IRQn);
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
}*/
