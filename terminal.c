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
#include "spi5.h"
#include "L3GD20.h"
#include "Board_LED.h"
#include <stdlib.h>
#include <stdio.h>

enum enTokenCategory
{
	EN_TC_OPERATION = 0,
	EN_TC_ARGUMENT1 = 1,
	EN_TC_ARGUMENT2 = 2,
	EN_TC_ARGUMENT3 = 3
};
enum enCommand
{
	EN_CMD_ECHO 		= 0,
	EN_CMD_TIM 			= 1,
	EN_CMD_HLP 			= 2,
	EN_CMD_DAC 			= 3,
	EN_CMD_ADC 			= 4,
	EN_CMD_SPI 			= 5,
	EN_CMD_PWM 			= 6,
	EN_CMD_GPIO 		= 7,
	EN_CMD_UNKNOWN 	= 8
};

char 		g_ach_Newline[] = "\r\n";
char 		g_ach_Operation[COMMANDS_LENGTH];
char 		g_ach_Arg[MAX_ARGUMENT_LENGTH];
char 		g_ach_Arg2[MAX_ARGUMENT_LENGTH];
int  		g_i_CommandNum = EN_TC_OPERATION;
uint8_t g_ui_OnOff=1U;
char 		g_ach_Mess[55];
char g_ach_CommandArr[NUMBER_OF_COMMANDS][COMMANDS_LENGTH] =
{
  { "echo" },{ "tim" },{ "hlp" },
	{ "dac" },{ "adc" },{ "spi" },
	{ "pwm" }, {"gpio"}
};
void TERMINAL_Init(void)
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
	TIMER_Start();
	LED_Initialize();
	MX_SPI5_Init();
	L3GD20_Init();
	HAL_Delay(1000u);
	UART_Transmit((uint8_t*)"Welcome to our STM32F429 terminal\n",34,TRANSMIT_TIMEOUT);
	UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
	UART_Transmit((uint8_t*)"Enter \"hlp\" to see the list of available commands",49,TRANSMIT_TIMEOUT);
	UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
}
void TERMINAL_Aliases(void)
{
	sprintf(g_ach_Mess,"Command not found, maybe you mean: ");
  UART_Transmit((uint8_t*)g_ach_Mess, strlen(g_ach_Mess), TRANSMIT_TIMEOUT);
	memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
  for (int i = 0; i<NUMBER_OF_COMMANDS; i++)
  {
    if (strpbrk(g_ach_Operation, g_ach_CommandArr[i]) != NULL)
    {
      UART_Transmit((uint8_t*)g_ach_CommandArr[i], strlen(g_ach_CommandArr[i]), TRANSMIT_TIMEOUT);
			UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
    }
  }
}

void TERMINAL_Echo()
{
	UART_Transmit((uint8_t*)g_ach_Arg, strlen(g_ach_Arg), TRANSMIT_TIMEOUT);
	UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
	UART_Transmit((uint8_t*)g_ach_Arg2, strlen(g_ach_Arg2), TRANSMIT_TIMEOUT);
	UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
}
void TERMINAL_Help(void)
{
  for (int i = 0; i<NUMBER_OF_COMMANDS; i++)
  {
    UART_Transmit((uint8_t*)g_ach_CommandArr[i], strlen(g_ach_CommandArr[i]), TRANSMIT_TIMEOUT);
    UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
  }
}


void TERMINAL_Parse(void)
{
	char 	l_ach_Line[MAX_ARGUMENT_LENGTH+1];
	char *l_pch_Token;
	int 	l_i_Category=0;
  UART_Receive((uint8_t*)l_ach_Line, MAX_ARGUMENT_LENGTH, RECEIVE_TIMEOUT);
  l_pch_Token = strtok(l_ach_Line," \t\n\r");
  while(l_pch_Token!=NULL)
  {
    switch (l_i_Category)
    {
      case EN_TC_OPERATION:
      {
        strcpy(g_ach_Operation, l_pch_Token);
        break;
      }
      case EN_TC_ARGUMENT1:
      {
        strcpy(g_ach_Arg, l_pch_Token);
        break;
      }
      case EN_TC_ARGUMENT2:
      {
        strcpy(g_ach_Arg2, l_pch_Token);
        break;
      }
			case EN_TC_ARGUMENT3:
      {
        break;
      }
    }
		l_i_Category++;
		l_pch_Token=strtok(NULL, " \t\n\r");
  }
		for(g_i_CommandNum=0;g_i_CommandNum<NUMBER_OF_COMMANDS;g_i_CommandNum++)
		{
			if(l_ach_Line[0]==0)
			{
				g_i_CommandNum=-1;
				break;
			}
			if (strcmp (g_ach_CommandArr[g_i_CommandNum], g_ach_Operation)==0)
			{
				memset(l_ach_Line,NULL,sizeof(l_ach_Line));
				break;
			}
		}
}
void TERMINAL_ExecCommand(void)
{
  switch (g_i_CommandNum)
  {
		case EN_CMD_ECHO:
		{
			TERMINAL_Echo();
			break;
		}
		case EN_CMD_TIM:
		{
			sprintf(g_ach_Mess,"Time from program start:%f",TIMER_GetTime());
			UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);		
			UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);			
			memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
			break;
		}
		case EN_CMD_HLP:
		{
			TERMINAL_Help();
			break;
		}
		case EN_CMD_DAC:
		{
			float l_fl_ValVolt;
			sscanf(g_ach_Arg,"%f",&l_fl_ValVolt);
			DAC_Start();
			DAC_SetValue(l_fl_ValVolt);
			sprintf(g_ach_Mess,"DAC value:%d",DAC_GetValue());
			UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
			UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
			memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
			DAC_Stop();
			break;
		}
		case EN_CMD_ADC:
		{
			uint32_t l_ui_Timeout;
			sscanf(g_ach_Arg,"%u",&l_ui_Timeout);
			sprintf(g_ach_Mess,"ADC value: %d",ADC_GetValue(l_ui_Timeout));
			UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
			UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
			memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
			break;
		}
	  case EN_CMD_SPI:
		{
			float 		l_fl_Curr = TIMER_GetTime();
			uint32_t 	l_ui_DelayTime=1000U;
			uint32_t 	l_ui_NumberOfIteration=5U;
			L3GD20_Init();
			HAL_Delay(l_ui_DelayTime);
			for(int i=0;i<l_ui_NumberOfIteration;i++)
			{
				sprintf(g_ach_Mess,"X:%f",L3GD20_GetAngularRateX(SENSITIVITY_NONE));
				UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
				UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
				
				sprintf(g_ach_Mess,"Y:%f",L3GD20_GetAngularRateY(SENSITIVITY_NONE));
				UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
				UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
				
				sprintf(g_ach_Mess,"Z:%f",L3GD20_GetAngularRateZ(SENSITIVITY_NONE));
				UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
				UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
				
				UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
				HAL_Delay(l_ui_DelayTime);
			}
			memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
			L3GD20_DeInit();
			break;
		}
	  case EN_CMD_PWM:
		{
			uint32_t l_ui_DutyCycle;
			sscanf(g_ach_Arg,"%u",&l_ui_DutyCycle);
			if(g_ui_OnOff==1U)
			{
				PWM_Start(l_ui_DutyCycle);
				sprintf(g_ach_Mess,"PWM was started");
				g_ui_OnOff=0U;
			}
			else if(g_ui_OnOff==0U)
			{
				PWM_Stop();
				sprintf(g_ach_Mess,"PWM was stopped");
				g_ui_OnOff=1U;
			}
			else
			{
				sprintf(g_ach_Mess,"Something is wrong. Please enter hlp for more details");
			}
			UART_Transmit((uint8_t*)g_ach_Mess, strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
			UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
			memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
			break;
		}
		case EN_CMD_GPIO:
		{
			uint32_t 	l_ui_PinNumber;
			uint32_t 	l_ui_RealPin=1;
			char 			l_ch_Port;
			sscanf(g_ach_Arg,"%u",&l_ui_PinNumber);
			sscanf(g_ach_Arg2,"%c",&l_ch_Port);
			for(int i=0;i<l_ui_PinNumber;i++)
			{
				l_ui_RealPin*=2;
			}
			uint8_t status=0u;
			switch(l_ch_Port)
			{
				case 'a':
				{
					HAL_GPIO_TogglePin(GPIOA, l_ui_RealPin);
					status++;
					break;
				}
				case 'b':
				{
					HAL_GPIO_TogglePin(GPIOB, l_ui_RealPin);
					status++;
					break;
				}
				case 'c':
				{
					HAL_GPIO_TogglePin(GPIOC, l_ui_RealPin);
					status++;
					break;
				}
				case 'd':
				{
					HAL_GPIO_TogglePin(GPIOD, l_ui_RealPin);
					status++;
					break;
				}
				case 'e':
				{
					HAL_GPIO_TogglePin(GPIOE, l_ui_RealPin);
					status++;
					break;
				}
				case 'f':
				{
					HAL_GPIO_TogglePin(GPIOF, l_ui_RealPin);
					status++;
					break;
				}
				case 'g':
				{
					HAL_GPIO_TogglePin(GPIOG, l_ui_RealPin);
					status++;
					break;
				}
				case 'h':
				{
					HAL_GPIO_TogglePin(GPIOH, l_ui_RealPin);
					status++;
					break;
				}
			}
			if(status==0U)
			{
				sprintf(g_ach_Mess,"Pin P%c%u doesn't exist",l_ch_Port,l_ui_PinNumber);
			}
			else
			{
				sprintf(g_ach_Mess,"Pin P%c%u has been toggled",l_ch_Port,l_ui_PinNumber);
			}
			UART_Transmit((uint8_t*)g_ach_Mess,strlen(g_ach_Mess),TRANSMIT_TIMEOUT);
			UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
			memset(g_ach_Mess,NULL,sizeof(g_ach_Mess));
			break;
		}
		case EN_CMD_UNKNOWN:
		{
			TERMINAL_Aliases();
		}
  }
}
