#include "stm32f4xx_hal.h"
#include "gpio.h"

/** 
* @brief GPIO Init structure 
*/ 
GPIO_InitTypeDef GPIO_InitStruct;

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes clocks for GPIO ports and sets up settings for PC1 pin

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);


  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

