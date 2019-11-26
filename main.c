#include "usart_IO.h"




int main(void)
{
	USART_HandleTypeDef huart2;
	uint8_t to = 49U;
uart_dma_init(&huart2);
uart_gpio_init();
uart_init(&huart2);

	while(1)
	{
		return HAL_USART_Transmit_DMA(&huart2,&to,sizeof(to));
		//uart_send(&to, sizeof(uint8_t));
	}
	return 0;
}


