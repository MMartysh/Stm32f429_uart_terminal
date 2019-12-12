#include "main.h"
#include "string.h"
#include "uart.h"
#include "dma.h"
#include "gpio.h"
#include "rcc.h"
#include "pwm.h"
#include "adc.h"
#include "dac.h"

extern TIM_HandleTypeDef htim1;
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

	
char getDate[30];
uint32_t valADC=0;
uint32_t dac_val=0;
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	startPWM();
	//startADC();
	startDAC();
	char Date[30]="Hello\r\t";
	
	
  
  while (1)
  {
		startDAC();
		setValue(2.2);
		dac_val=HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
		stopDAC();
		valADC=getValueADC(5);
    /*uartTransmit((uint8_t*)Date, strlen(Date),10);
		HAL_UART_Receive(&huart1,(uint8_t*)getDate, 10,0xffff);
	  HAL_UART_Transmit(&huart1, (uint8_t*)getDate, strlen(getDate),10);*/

    
  }

}

void Error_Handler(void)
{
 
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

