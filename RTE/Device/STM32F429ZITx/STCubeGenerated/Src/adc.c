#include "adc.h"

//adc handler

ADC_HandleTypeDef hadc1;


/**
* function which initializes adc peripheral according to the specified parameters
*/


void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
 
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * function which starts adc
 */

void ADC_Start(void)
{
	HAL_ADC_Start(&hadc1);
};

/**
 * function which stops adc
 */

void ADC_Stop(void)
{
	HAL_ADC_Stop(&hadc1);
}

/**
 *function which returns adc value
 *@param {uint32_t} time
 *@return {uint32_t}
 */

uint32_t ADC_GetValue(uint32_t p_ui_Time)
{
	while(1)
	{
		ADC_Start();
		if(HAL_ADC_PollForConversion(&hadc1,p_ui_Time)==HAL_OK)
		{
			return HAL_ADC_GetValue(&hadc1);
		}
		ADC_Stop();
	}
}
