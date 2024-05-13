#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "pwm_ctrl.h"
#include "terminal.h"
//pwm handler

TIM_HandleTypeDef htim1;

/**
* @brief  TIM Output Compare Configuration Structure
*/

TIM_OC_InitTypeDef sConfigOC = {0};


 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes clocks for GPIO ports and sets up settings for PC1 pin
*/
/* ----------------------------------------------------------------------------
 */
void pwmInit(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 16;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 100;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 99;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Starts PWM on Channel 1 with given duty cycle

 @param[in]     dutyCycle PWM duty cycle in percents 

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void pwmStart(uint32_t dutyCycle)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	htim1.Instance->CCR1 = dutyCycle;
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Stops PWM on Channel 1 
*/
/* ----------------------------------------------------------------------------
 */
void pwmStop(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Return ADC value

 @param[in]     argc nummber of arguments
 @param[in]     argv arguments array

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalPwmGenerateOutput(uint8_t argc, char **argv)
{
    (void)argc;
    static bool onOffSwitch = true;
    uint32_t dutyCycle;
    if(strtol(argv[1], NULL, dutyCycle) == 0)
    {
        printf("Invalid argument\n");
        return false;
    }
    if(onOffSwitch)
    {
        pwmStart(dutyCycle);
        printf("PWM started\n");
        onOffSwitch = false;
    }
    else
    {
        pwmStop();
        printf("PWM stopped\n");
        onOffSwitch = true;
    }
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
void terminalPwmInit(void)
{
    static commandStruct pwmCommand = 
    { 
        .name = "pwm",
        .description = "Generates PWM signal with given duty cycle. \n\n  pwm [arg1]\n"
                     "\t arg1 - desired duty cycle in percents",
        .callback = terminalPwmGenerateOutput,
        .next = NULL 
    };
    terminalAddCommand(&pwmCommand);
}
