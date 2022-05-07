#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "timer_ctrl.h"
#include "terminal.h"

//dac handler

TIM_HandleTypeDef htim3;

//current time (from start programme)
static float currentTime;

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Interrupt handler

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void SysTick_Handler(void)
{
  HAL_IncTick();
	currentTime=(HAL_GetTick());
  currentTime /= (float)636.74694822;//converting ticks into seconds
  static uint8_t counter = 0;
  if(++counter == 200)
  {
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
  }
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes TIM3

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void timerInit(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes TIM3

 @param         None.

 @return        Status of the operation. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef timerStart(void)
{
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(&htim3);
	return HAL_TIM_Base_Start_IT(&htim3);
}


/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Dumb function to print current time variable (maybe replace this function later)

 @param         None.

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalTimerGetTime(uint8_t argc, char **argv)
{
  (void)argc;
  (void)argv;
  printf("Time from program start: %f\n", currentTime);
  return true;
}

__attribute__((constructor))
void terminalTimerInit(void)
{
  static commandStruct timerCommand = 
  { 
          .name = "time",
          .description = "Returns time from program start. \n\n  time ",
          .callback = terminalTimerGetTime,
          .next = NULL 
  };
  terminalAddCommand(&timerCommand);
}
