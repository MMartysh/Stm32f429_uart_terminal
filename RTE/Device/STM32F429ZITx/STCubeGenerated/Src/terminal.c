#include "terminal.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "string.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "dac.h"
#include <stdlib.h>
#include <stdio.h>

enum tokenCategory
{
	OPERATION = 0,
	ARGUMENT1 = 1,
	ARGUMENT2 = 2,
};
enum Command
{
	ECHO = 0,
	TIM = 1,
	HLP = 2,
	DAC_ = 3,
	ADC_ = 4,
	SPI = 5,
	PWM = 6,
	GPIO = 7
};
char newline[] = "\r\n";
char operation[COMMANDS_LENGTH];
char arg[MAX_ARGUMENT_LENGTH];
char arg2[MAX_ARGUMENT_LENGTH];
int commandNum = OPERATION;
char commandsArr[NUMBER_OF_COMMANDS][COMMANDS_LENGTH] =
{
  { "echo" },{ "tim" },{ "hlp" },
	{ "dac" },{ "adc" },{ "spi" },
	{ "pwm" }, {"gpio"}
};
void terminalInit(void)
{
	HAL_Init();
  SystemClock_Config();
	SysTick_Config(SystemCoreClock /1000);
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	MX_TIM3_Init();
	startPWM(TIM_CHANNEL_1);
	startTimer();
}

void aliases(void)
{
	char str[]="Command not found, maybe you mean: ";
  uartTransmit((uint8_t*)str, strlen(str), TRANSMIT_TIMEOUT);
  for (int i = 0; i<NUMBER_OF_COMMANDS; i++)
  {
    if (strpbrk(operation, commandsArr[i]) != NULL)
    {
      uartTransmit((uint8_t*)commandsArr[i], strlen(commandsArr[i]), TRANSMIT_TIMEOUT);
    }
  }
}

void echo()
{
	uartTransmit((uint8_t*)arg, strlen(arg), TRANSMIT_TIMEOUT);
	uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
	uartTransmit((uint8_t*)arg2, strlen(arg), TRANSMIT_TIMEOUT);
	uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
}
void help(void)
{
  for (int i = 0; i<NUMBER_OF_COMMANDS; i++)
  {
    uartTransmit((uint8_t*)commandsArr[i], strlen(commandsArr[i]), TRANSMIT_TIMEOUT);
    uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
  }
}


void parse(void)
{
	char line[MAX_ARGUMENT_LENGTH];
  uartReceive((uint8_t*)line, MAX_ARGUMENT_LENGTH, RECEIVE_TIMEOUT);
  char *token;
	int category=0;
  token = strtok(line," \t\n");
  while(token!=NULL)
  {
    switch (category)
    {
      case OPERATION:
      {
        strcpy(operation, token);
        break;
      }
      case ARGUMENT1:
      {
        strcpy(arg, token);
        break;
      }
      case ARGUMENT2:
      {
        strcpy(arg2, token);
        break;
      }
    }
		category++;
		token=strtok(NULL, " \t\n");
  }
	for(commandNum=0;commandNum<NUMBER_OF_COMMANDS;commandNum++)
  {
    if (strcmp (commandsArr[commandNum], operation)==0)
    {
			memset(line,0,sizeof(line));
			memset(operation,0,sizeof(operation));
      break;
    }
  }
}
void execCommand(void)
{
  switch (commandNum)
  {
		case ECHO:
		{
			echo();
			break;
		}
		case TIM:
		{
			char currentTime[5];
			sprintf(currentTime,"%f",getTime());
			uartTransmit((uint8_t*)&currentTime,strlen(currentTime),TRANSMIT_TIMEOUT);			
			break;
		}
		case HLP:
		{
			help();
			break;
		}
		case DAC_:
		{
			char DACval[5];
			float valVolt;
			sscanf(arg,"%f",&valVolt);
			startDAC();
			setValue(valVolt);
			sprintf(DACval,"%d",getValue());
			uartTransmit((uint8_t*)DACval,3,10);
			stopDAC();
			break;
		}
		case ADC_:
		{
			char ADCval[5];
			uint32_t timeout;
			sscanf(arg,"%u",&timeout);
			sprintf(ADCval,"%d",getValueADC(timeout));
			uartTransmit((uint8_t*)ADCval,strlen(ADCval),TRANSMIT_TIMEOUT);
			break;
		}
	  case SPI:
		{
			//spi(); 
			break;
		}
	  case PWM:
		{
			uint32_t pulse;
			sscanf(arg,"%u",&pulse);
			if(strcmp (arg2, "0") == 0)
			{
				uint32_t channel;
				sscanf(arg2,"%u",&channel);
				stopPWM(channel);
				MX_TIM1_Init(pulse,channel);
				startPWM(channel);
			}
			else
			{
				char pulseConverted[5];
				sprintf(pulseConverted,"%d",getPulse(pulse));
				uartTransmit((uint8_t*)pulseConverted,strlen(pulseConverted),TRANSMIT_TIMEOUT);
			}
			break;
		}
		case GPIO:
		{
			uint32_t pin;
			char port;
			sscanf(arg,"%u",&pin);
			sscanf(arg2,"%c",&port);
			switch(port)
			{
				case 'a':
				{
					HAL_GPIO_TogglePin(GPIOA, pin);
					break;
				}
				case 'b':
				{
					HAL_GPIO_TogglePin(GPIOB, pin);
					break;
				}
				case 'c':
				{
					HAL_GPIO_TogglePin(GPIOC, pin);
					break;
				}
				case 'd':
				{
					HAL_GPIO_TogglePin(GPIOD, pin);
					break;
				}
				case 'e':
				{
					HAL_GPIO_TogglePin(GPIOE, pin);
					break;
				}
				case 'f':
				{
					HAL_GPIO_TogglePin(GPIOF, pin);
					break;
				}
				case 'g':
				{
					HAL_GPIO_TogglePin(GPIOG, pin);
					break;
				}
				case 'h':
				{
					HAL_GPIO_TogglePin(GPIOH, pin);
					break;
				}
			}
			break;
		}
		/*default:
		{
			aliases();
		}*/
  }
}
