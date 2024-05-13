#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "stm32f4xx_hal.h"
#include "gpio_ctrl.h"
#include "terminal.h"

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initialize clocks for GPIO ports
*/
/* ----------------------------------------------------------------------------
 */
void gpioInit(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         GPIO terminal command handler function

 @param[in]     argc nummber of arguments
 @param[in]     argv arguments array

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalGpio(uint8_t argc, char **argv)
{
    (void)argc;
    uint32_t pinNumber;
    char portName;
    static GPIO_TypeDef *gpioPorts[] = 
    {
        GPIOA, GPIOB, GPIOC, GPIOD,
        GPIOE, GPIOF, GPIOG, GPIOH
    };

    if(!terminalGetInt(argv[1], &pinNumber) || strlen(argv[2]) > 1)
    {
        printf("Invalid argument\n");
        return false;
    }
    portName = tolower(argv[2][0]);
    if(pinNumber > 15)
    {
        printf("Pin doesn't exist\n");
        return false;
    }
    if(portName < 'a' || portName > 'h')
    {
        printf("Non-existent port\n");
        return false;
    }

    HAL_GPIO_TogglePin(gpioPorts[portName - 97], 1 << pinNumber);
    printf("Pin has been toggled\n");
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
void terminalGpioInit(void)
{
    static commandStruct gpioCommand = 
    { 
        .name = "gpio",
        .description = "Changes state of the given GPIO pin. \n\n  gpio [arg1][arg2]"
                     "arg1 - pin number"
                     "arg2 - pin port name( letter from a to f)",
        .callback = terminalGpio,
        .next = NULL 
    };
    terminalAddCommand(&gpioCommand);
}

