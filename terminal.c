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
	ARGUMENT3 = 3
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
uint8_t onOff=1U;
char mess[55];
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
	writeSPI(L3GD20_CTRL_REG1_ADDR,0x0F);
	HAL_Delay(1000u);
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
	sprintf(mess,"Command not found, maybe you mean: ");
  uartTransmit((uint8_t*)mess, strlen(mess), TRANSMIT_TIMEOUT);
	memset(mess,NULL,sizeof(mess));
  for (int i = 0; i<NUMBER_OF_COMMANDS; i++)
  {
    if (strpbrk(operation, commandsArr[i]) != NULL)
    {
      uartTransmit((uint8_t*)commandsArr[i], strlen(commandsArr[i]), TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
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
			case ARGUMENT3:
      {
        break;
      }
    }
		category++;
		token=strtok(NULL, " \t\n\r");
  }
		for(commandNum=0;commandNum<NUMBER_OF_COMMANDS;commandNum++)
		{
			if(line[0]==0)
			{
				commandNum=-1;
				break;
			}
			if (strcmp (commandsArr[commandNum], operation)==0)
			{
				memset(line,NULL,sizeof(line));
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
			sprintf(mess,"Time from program start:%f",getTime());
			uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);		
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);			
			memset(mess,NULL,sizeof(mess));
			break;
		}
		case HLP:
		{
			help();
			break;
		}
		case DAC_:
		{
			float valVolt;
			sscanf(arg,"%f",&valVolt);
			startDAC();
			setValue(valVolt);
			sprintf(mess,"DAC value:%d",getValue());
			uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			memset(mess,NULL,sizeof(mess));
			stopDAC();
			break;
		}
		case ADC_:
		{
			uint32_t timeout;
			sscanf(arg,"%u",&timeout);
			sprintf(mess,"ADC value: %d",getValueADC(timeout));
			uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			memset(mess,NULL,sizeof(mess));
			break;
		}
	  case SPI:
		{
			float curr = getTime();
			uint32_t timeout=1000U;
			uint32_t nums=5U;
			uint8_t ctrl1=0x0F;
			writeSPI(L3GD20_CTRL_REG1_ADDR,ctrl1);
			HAL_Delay(timeout);
			sprintf(mess,"CTRL REG 1 value:%u",readSPI(L3GD20_CTRL_REG1_ADDR));
			uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			for(int i=0;i<nums;i++)
			{
				sprintf(mess,"X:%f",L3GD20_GetAngularRateX(SENSITIVITY_NONE));
				uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
				uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				
				sprintf(mess,"Y:%f",L3GD20_GetAngularRateY(SENSITIVITY_NONE));
				uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
				uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				
				sprintf(mess,"Z:%f",L3GD20_GetAngularRateZ(SENSITIVITY_NONE));
				uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
				uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				
				uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
				HAL_Delay(timeout);
			}
			memset(mess,NULL,sizeof(mess));
			ctrl1=0x07;
			writeSPI(L3GD20_CTRL_REG1_ADDR,ctrl1);
			break;
		}
	  case PWM:
		{
			uint32_t cycle;
			sscanf(arg,"%u",&cycle);
			if(onOff==1U)
			{
				startPWM(cycle);
				sprintf(mess,"PWM was started");
				onOff=0U;
			}
			else if(onOff==0U)
			{
				stopPWM();
				sprintf(mess,"PWM was stopped");
				onOff=1U;
			}
			else
			{
				sprintf(mess,"Something is wrong. Please enter hlp for more details");
			}
			uartTransmit((uint8_t*)mess, strlen(mess),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			memset(mess,NULL,sizeof(mess));
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
			uint8_t status=0u;
			switch(port)
			{
				case 'a':
				{
					HAL_GPIO_TogglePin(GPIOA, pin);
					status++;
					break;
				}
				case 'b':
				{
					HAL_GPIO_TogglePin(GPIOB, pin);
					status++;
					break;
				}
				case 'c':
				{
					HAL_GPIO_TogglePin(GPIOC, pin);
					status++;
					break;
				}
				case 'd':
				{
					HAL_GPIO_TogglePin(GPIOD, pin);
					status++;
					break;
				}
				case 'e':
				{
					
					status++;HAL_GPIO_TogglePin(GPIOE, pin);
					break;
				}
				case 'f':
				{
					HAL_GPIO_TogglePin(GPIOF, pin);
					status++;
					break;
				}
				case 'g':
				{
					HAL_GPIO_TogglePin(GPIOG, pin);
					status++;
					break;
				}
				case 'h':
				{
					HAL_GPIO_TogglePin(GPIOH, pin);
					status++;
					break;
				}
			}
			if(status==0U)
			{
				sprintf(mess,"Pin P%c%u doesn't exist",port,pinnum);
			}
			else
			{
				sprintf(mess,"Pin P%c%u has been toggled",port,pinnum);
			}
			uartTransmit((uint8_t*)mess,strlen(mess),TRANSMIT_TIMEOUT);
			uartTransmit((uint8_t*)newline, strlen(newline), TRANSMIT_TIMEOUT);
			memset(mess,NULL,sizeof(mess));
			break;
		}
		case UNKNOWN:
		{
			aliases();
		}
  }
}
