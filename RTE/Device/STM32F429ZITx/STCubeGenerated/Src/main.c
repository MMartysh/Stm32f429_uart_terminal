#include "main.h"
#include "terminal.h"
#include "string.h"

extern UART_HandleTypeDef huart1;

char Rx_buffer[100];
unsigned char Rx_indx, Rx_data[2], Transfer_cplt;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		uint8_t i;
		if (huart->Instance == USART1)             //is current uart?
		{
			if (Rx_indx == 0)                       //if data is not being received
			{
				for (i=0;i<100;i++)
				{
					Rx_buffer[i]=0;											// clear Rx_Buffer before receiving new data
				}
			}
			if (Rx_data[0]!=13)                     //if received data different from ascii 13 (enter)
			{
				Rx_buffer[Rx_indx++]= Rx_data[0];     // store data in buffer
			}
			else 																	// if received data = 13
			{
				Rx_indx= 0;
				if (!(strcmp(Rx_buffer, "goto2")))
				{
					//lcd_send_cmd (0xc0);
				}
				else if (!(strcmp(Rx_buffer, "clear")))
				{
					//lcd_send_cmd (0x01);
					//lcd_send_cmd (0x80);
				}
				else /*lcd_send_string (Rx_buffer)*/;
			}
			HAL_UART_Receive_IT (&huart1, Rx_data, 1);     // activate receive
		}
	}


int main(void)
{
  terminalInit();  
	HAL_UART_Receive_IT (&huart1, Rx_data, 1);
	//__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  while (1)
  {
	//parse();
	//execCommand();
  }
}
void Error_Handler(void)
{
	//LED_Initialize();
	//LED_On(0);
	while(1)
	{
	}
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */