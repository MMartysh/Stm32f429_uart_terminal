#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
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
#include "at24xx.h"
#include "l3gd20_ctrl.h"
#include "si7021_ctrl.h"
#include "max7219_ctrl.h"
#include "timer_ctrl.h"
#include "terminal.h"
#include "lcd1602a_ctrl.h"

static void ledTimerFunc(void);
static void i2cSearchDevices(void);

static timerHandler ledTimer =
{
    .periodMs = 1000, // 1 second period
    .callback = ledTimerFunc
};
    
static void ledTimerFunc(void)
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
static void i2cSearchDevices(void)
{
	int i;
	uint8_t ch[2];

	printf("Probing available I2C devices...\r\n");
	printf("\r\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
	printf("\r\n====================================================");
	for (i = 0; i <= 0x7F; i++) 
    {
		if (!(i & 0x0F)) 
        {
			printf("\r\n%02X  ", i >> 4);
		}
		if ((i <= 7) || (i > 0x78)) 
        {
			printf("   ");
			continue;
		}
		if (i2cRead(i, ch, 1 + (i == 0x48)) > 0) 
        {
			printf(" %02X", i);
		}
		else 
        {
			printf(" --");
		}
	}
	printf("\r\n");
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
    spiInit(SPI_CTRL_MAX7219);
    spiInit(SPI_CTRL_ACC);
    l3gd20Init();
    terminalInit(); 
    i2cInit();
    si7021WriteResolution(SI7021_RES_11_RH_11_TEMP);
    timerAdd(&ledTimer);
    lcdInit(&lcd, i2cGetHandle(), 0x27, 20, 4);
    lcdBacklight(&lcd, 1);
    i2cSearchDevices();
    while (1)
    {

        timerPerformCheck();
        terminalGetChar();
    }
}
