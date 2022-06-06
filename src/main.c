#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "board.h"
#include "uart_ctrl.h"
#include "gpio_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "i2c_ctrl.h"
#include "pwm_ctrl.h"
#include "rcc_ctrl.h"
#include "spi_ctrl.h"
#include "l3gd20_ctrl.h"
#include "si7021_ctrl.h"
#include "timer_ctrl.h"
#include "terminal.h"
void ledTimerFunc(void)
{
    static uint8_t ledCounter = 0;
    //perform fast parity check
    if(++ledCounter & 1)
    {
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
    }
    else
    {
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
    }
}
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    gpioInit();
    boardInitPins();
    uartInit();
    adcInit();
    dacInit();
    timerInit();
    pwmInit();
    spiInit();
    l3gd20Init();
    terminalInit(); 
    i2cInit();
    si7021ReadId();
    static timerHandler ledTimer;
    ledTimer.periodMs = 1000; // 1 second period
    ledTimer.callback = ledTimerFunc;
    timerAdd(&ledTimer);
    while (1)
    {
        timerPerformCheck();
        terminalGetChar();
    }
}
