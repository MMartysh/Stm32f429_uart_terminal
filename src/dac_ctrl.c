#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "dac_ctrl.h"
#include "timer_ctrl.h"
#include "terminal.h"
//dac handler

DAC_HandleTypeDef hdac;

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes DAC on channel 1

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void dacInit(void)
{
  DAC_ChannelConfTypeDef sConfig = {0};

  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Starts DAC on channel 1

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef dacStart(void)
{
	return HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Stops DAC on channel 1

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef dacStop(void)
{
	return HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Sets DAC signal value on channel 1

 @param[in]     p_fl_ValVolt desired DAC signal value.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef dacSetValue(uint32_t voltage)
{
	dacStart();
	uint32_t valByte = voltage * 85;
	dacStop();
	return HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, valByte);
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Starts ADC on channel 1

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
uint32_t dacGetValue(void)
{
	return HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Generates voltage output

 @param         None.

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalDacOutput(uint8_t argc, char **argv)
{
  (void)argc;
  uint32_t valVolt;
  strtol(argv[1], NULL, valVolt);
  dacStart();
  dacSetValue(valVolt);
  printf("DAC value: %d\n", dacGetValue());
  dacStop();
  return true;
}

__attribute__((constructor))
void terminalDacInit(void)
{
  static commandStruct dacCommand = 
  { 
          .name = "dac",
          .description = "Generates given voltage output. \n\n  dac [arg1]\n"
                          "\targ1 - voltage value to generate(from 0 to 3,3V)",
          .callback = terminalDacOutput,
          .next = NULL 
  };
  terminalAddCommand(&dacCommand);
}
