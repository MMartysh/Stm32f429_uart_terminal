#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "adc_ctrl.h"
#include "terminal.h"
//adc handler

ADC_HandleTypeDef adcPeriphHandler;


 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes ADC1
*/
/* ----------------------------------------------------------------------------
 */
void adcInit(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    adcPeriphHandler.Instance = ADC1;
    adcPeriphHandler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adcPeriphHandler.Init.Resolution = ADC_RESOLUTION_8B;
    adcPeriphHandler.Init.ScanConvMode = DISABLE;
    adcPeriphHandler.Init.ContinuousConvMode = DISABLE;
    adcPeriphHandler.Init.DiscontinuousConvMode = DISABLE;
    adcPeriphHandler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adcPeriphHandler.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adcPeriphHandler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adcPeriphHandler.Init.NbrOfConversion = 1;
    adcPeriphHandler.Init.DMAContinuousRequests = DISABLE;
    adcPeriphHandler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&adcPeriphHandler) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&adcPeriphHandler, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Starts ADC on channel 1
*/
/* ----------------------------------------------------------------------------
 */
void adcStart(void)
{
	HAL_ADC_Start(&adcPeriphHandler);
};

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief        Stops ADC on channel 1
*/
/* ----------------------------------------------------------------------------
 */
void adcStop(void)
{
	HAL_ADC_Stop(&adcPeriphHandler);
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Reads value from ADC channel 1 in polling blocking mode

 @param[in]     timeout convertion timeout.

 @return        ADC value on channel 1. 
*/
/* ----------------------------------------------------------------------------
 */
uint32_t adcGetValue(uint32_t timeout)
{
	while(1)
	{
		adcStart();
		if(HAL_ADC_PollForConversion(&adcPeriphHandler, timeout) == HAL_OK)
		{
			return HAL_ADC_GetValue(&adcPeriphHandler);
		}
		adcStop();
	}
}
/* ----------------------------------------------------------------------------
 */
/*!
 @brief         ADC terminal command handler function

 @param[in]     argc nummber of arguments
 @param[in]     argv arguments array

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalAdcGetValue(uint8_t argc, char **argv)
{
    (void)argc;
    uint32_t timeout;
    if(!terminalGetInt(argv[1], &timeout))
    {
        return false;
    }
    printf("ADC value: %d\n", adcGetValue(timeout));
    return true;
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Add terminal command handle to list
*/
/* ----------------------------------------------------------------------------
 */
__attribute__((constructor))
void terminalAdcInit(void)
{
    static commandStruct adcCommand = 
    { 
        .name = "adc",
        .description = "Shows ADC value. \n\n  adc [arg1]\n"
                      "\t arg1 - read timeout",
        .callback = terminalAdcGetValue,
        .next = NULL 
    };
    terminalAddCommand(&adcCommand);
}