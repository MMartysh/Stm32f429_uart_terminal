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
#include "spi5.h"
#include "Board_LED.h"
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
	GPIO = 7,
	UNKNOWN = 8
};
char newline[] = "\r\n";
char operation[COMMANDS_LENGTH];
char arg[MAX_ARGUMENT_LENGTH];
char arg2[MAX_ARGUMENT_LENGTH];
int commandNum = OPERATION;

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
	MX_TIM1_Init();
	startTimer();
	LED_Initialize();
	MX_SPI5_Init();
	uint8_t ctrl1=0x0F;
	writeSPI(L3GD20_CTRL_REG1_ADDR,ctrl1);
	uartTransmit((uint8_t*)"Welcome to our STM32F429 terminal\n",34,TRANSMIT_TIMEOUT);
	uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
	uartTransmit((uint8_t*)"Enter \"hlp\" to see the list of available commands",49,TRANSMIT_TIMEOUT);
	uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
}
char commandsArr[NUMBER_OF_COMMANDS][COMMANDS_LENGTH] =
{
  { "echo" },{ "tim" },{ "hlp" },
	{ "dac" },{ "adc" },{ "spi" },
	{ "pwm" }, {"gpio"}
};
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
	uartTransmit((uint8_t*)arg2, strlen(arg2), TRANSMIT_TIMEOUT);
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
	char line[MAX_ARGUMENT_LENGTH+1];
  uartReceive((uint8_t*)line, MAX_ARGUMENT_LENGTH, RECEIVE_TIMEOUT);
  char *token;
	int category=0;
  token = strtok(line," \t\n\r");
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
		token=strtok(NULL, " \t\n\r");
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
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);			
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
			uartTransmit((uint8_t*)DACval,strlen(DACval),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
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
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			break;
		}
	  case SPI:
		{
			float curr = getTime();
			uint32_t timeout;
			char d[6];
			sscanf(arg,"%u",&timeout);
			while((getTime()-curr)<timeout)
			{
					sprintf(d,"%f",L3GD20_GetAngularRateX(SENSITIVITY_NONE));
					uartTransmit((uint8_t*)d,strlen(d),TRANSMIT_TIMEOUT);
					uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				
					sprintf(d,"%f",L3GD20_GetAngularRateY(SENSITIVITY_NONE));
					uartTransmit((uint8_t*)d,strlen(d),TRANSMIT_TIMEOUT);
					uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				
					
					sprintf(d,"%f",L3GD20_GetAngularRateZ(SENSITIVITY_NONE));
					uartTransmit((uint8_t*)d,strlen(d),TRANSMIT_TIMEOUT);
					uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				
					uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			}
			break;
		}
	  case PWM:
		{
			uint32_t onOff;
			uint32_t cycle;
			char mess[55];
			sscanf(arg,"%u",&onOff);
			sscanf(arg2,"%u",&cycle);
			if(onOff==1U)
			{
				startPWM(cycle);
				strcpy(mess,"PWM was started");
			}
			else if(onOff==0U)
			{
				stopPWM();
				strcpy(mess, "PWM was stopped");
			}
			else
			{
				strcpy(mess,"Something is wrong. Please enter hlp for more details");
			}
			uartTransmit((uint8_t*)mess, strlen(mess),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			break;
		}
		case GPIO:
		{
			uint32_t pinnum;
			uint32_t pin=1;
			char port;
			sscanf(arg,"%u",&pinnum);
			sscanf(arg2,"%c",&port);
			for(int i=0;i<pinnum;i++)
			{
				pin*=2;
			}
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
  }
}
