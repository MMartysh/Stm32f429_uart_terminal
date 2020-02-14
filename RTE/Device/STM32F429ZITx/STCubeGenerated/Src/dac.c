#include "dac.h"

//dac handler

DAC_HandleTypeDef hdac;

/**
*Initialization dac peripheral according to the specified parameters
*/

void MX_DAC_Init(void)
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


/**
	*function which starts dac
	*@return {HAL_StatusTypeDef}
 */

HAL_StatusTypeDef DAC_Start(void)
{
	return HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}

/**
	*function which stops dac
	*@return {HAL_StatusTypeDef}
 */

HAL_StatusTypeDef DAC_Stop(void)
{
	return HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
}

/**
  *function which sets dac value
	*@param {float} valVolt
	*return {HAL_StatusTypeDef}
 */

HAL_StatusTypeDef DAC_SetValue(float p_fl_ValVolt)
{
	DAC_Start();
	uint8_t valByte;
	valByte=(uint8_t)((p_fl_ValVolt/(float)3.0)*255);
	DAC_Stop();
	return HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, valByte);
}

/**
	*function which returns dac value
	*return {uint32_t}
 */

uint32_t DAC_GetValue(void)
{
	return HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
}


