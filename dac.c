#include "dac.h"

DAC_HandleTypeDef hdac;
float valDac;

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

HAL_StatusTypeDef startDAC(void)
{
	return HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}
HAL_StatusTypeDef stopDAC(void)
{
	return HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
}

HAL_StatusTypeDef setValue(float valVolt)
{
	startDAC();
	uint8_t valByte;
	valByte=(uint8_t)((valVolt/(float)3.0)*255);
	stopDAC();
	return HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, valByte);
}
uint32_t getValue(void)
{
	return HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
}


