#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "board.h"
#include "uart_ctrl.h"
#include "gpio_ctrl.h"
#include "adc_ctrl.h"
#include "dac_ctrl.h"
#include "pwm_ctrl.h"
#include "rcc_ctrl.h"
#include "spi_ctrl.h"
#include "l3gd20_ctrl.h"
#include "timer_ctrl.h"
#include "dma_ctrl.h"
#include "terminal.h"

int main(void)
{
  HAL_Init();
  SystemClock_Config();
	SysTick_Config(SystemCoreClock / 1000);
  gpioInit();
  boardInitPins();
  dmaInit();
  uartInit();
	adcInit();
	dacInit();
	//timerInit();
	pwmInit();
	timerStart();
	spiInit();
	l3gd20Init();
  terminalInit(); 
  while (1)
  {
    terminalGetChar();
  }
}
