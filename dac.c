#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "dac.h"

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
void DAC_Init(void)
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
HAL_StatusTypeDef DAC_Start(void)
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
HAL_StatusTypeDef DAC_Stop(void)
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
HAL_StatusTypeDef DAC_SetValue(float p_fl_ValVolt)
{
	DAC_Start();
	uint8_t valByte;
	valByte = (uint8_t)((p_fl_ValVolt /( float)3.0) * 255);
	DAC_Stop();
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
uint32_t DAC_GetValue(void)
{
	return HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
}


