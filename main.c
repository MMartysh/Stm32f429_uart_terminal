#include "main.h"
#include "terminal.h"

int main(void)
{
  terminalInit();  
  while (1)
  {
	parse();
	execCommand();
  }
}
void Error_Handler(void)
{
 LED_Initialize();
 HAL_GPIO_WritePin(LED_PIN[13].port, LED_PIN[13].pin,            GPIO_PIN_SET);
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