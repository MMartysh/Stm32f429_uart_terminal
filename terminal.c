#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
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
#include "terminal.h"

/** 
  * @brief Command format structure (name of command, argument1, argument2, argument3)
  */
enum enTokenCategory
{
  EN_TC_OPERATION = 0,
  EN_TC_ARGUMENT1 = 1,
  EN_TC_ARGUMENT2 = 2,
  EN_TC_ARGUMENT3 = 3
};

/** 
  * @brief List of commands
  */
enum enCommand
{
  EN_CMD_ECHO     = 0,
  EN_CMD_TIM       = 1,
  EN_CMD_HLP       = 2,
  EN_CMD_DAC       = 3,
  EN_CMD_ADC       = 4,
  EN_CMD_SPI       = 5,
  EN_CMD_PWM       = 6,
  EN_CMD_GPIO     = 7,
  EN_CMD_UNKNOWN   = 8
};

const  char       g_ach_Newline[] = "\r\n";                               //New line
static char       g_ach_Operation[COMMANDS_LENGTH];                       //Command ame
static char       g_ach_Arg[MAX_ARGUMENT_LENGTH];                         //argument1
static char       g_ach_Arg2[MAX_ARGUMENT_LENGTH];                        //argument2
static int        g_i_CommandNum = EN_TC_OPERATION;                       // number of command
static const char g_ach_CommandArr[NUMBER_OF_COMMANDS][COMMANDS_LENGTH] = //List of commands
{
  { "echo" },{ "tim" },{ "hlp" },
  { "dac" },{ "adc" },{ "spi" },
  { "pwm" }, {"gpio"}
};

static void TERMINAL_Aliases(void);
static void TERMINAL_Help(void);
static void TERMINAL_Echo(void);

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Shows start message in terminal

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void TERMINAL_Init(void)
{
  HAL_Init();
  SystemClock_Config();
	SysTick_Config(SystemCoreClock /1000);
  GPIO_Init();
  DMA_Init();
  UART_Init();
	ADC1_Init();
	DAC_Init();
	//TIM3_Init();
	TIM1_Init();
	TIMER_Start();
	LED_Initialize();
	SPI5_Init();
	L3GD20_Init();
  //HAL_Delay(1000u);
  const static char l_ach_welcomingMessage[] = "Welcome to STM32F429 terminal app\n";
  const static char l_ach_hintMessage[] = "Enter \"hlp\" to see the list of available commands";
  UART_Transmit((uint8_t*)l_ach_welcomingMessage, sizeof(l_ach_welcomingMessage) / sizeof(l_ach_welcomingMessage[0]), TRANSMIT_TIMEOUT);
  UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
  UART_Transmit((uint8_t*)l_ach_hintMessage, sizeof(l_ach_hintMessage) / sizeof(l_ach_hintMessage[0]), TRANSMIT_TIMEOUT);
  UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Finds similar commands in case of wrong input

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void TERMINAL_Aliases(void)
{
  static char l_ach_Message[] = "Command not found, maybe you mean: ";
  UART_Transmit((uint8_t*)l_ach_Message, strlen(l_ach_Message), TRANSMIT_TIMEOUT);
  for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
  {
    if (strpbrk(g_ach_Operation, g_ach_CommandArr[i]) != NULL)
    {
      UART_Transmit((uint8_t*)g_ach_CommandArr[i], strlen(g_ach_CommandArr[i]), TRANSMIT_TIMEOUT);
      UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
    }
  }
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Returns arguments of the echo terminal command

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void TERMINAL_Echo()
{
  UART_Transmit((uint8_t*)g_ach_Arg, strlen(g_ach_Arg), TRANSMIT_TIMEOUT);
  UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
  UART_Transmit((uint8_t*)g_ach_Arg2, strlen(g_ach_Arg2), TRANSMIT_TIMEOUT);
  UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Shows list of available commands

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void TERMINAL_Help(void)
{
  for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
  {
    UART_Transmit((uint8_t*)g_ach_CommandArr[i], strlen(g_ach_CommandArr[i]), TRANSMIT_TIMEOUT);
    UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
  }
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Parses received string

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void TERMINAL_Parse(void)
{
  char  l_ach_Line[MAX_ARGUMENT_LENGTH+1];
  char *l_pch_Token;
  int   l_i_Category = 0;
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
    l_pch_Token = strtok(NULL, " \t\n\r");
  }
    for(g_i_CommandNum=0; g_i_CommandNum<NUMBER_OF_COMMANDS; g_i_CommandNum++)
    {
      if(l_ach_Line[0] == 0)
      {
        g_i_CommandNum = -1;
        break;
      }
      if (strcmp (g_ach_CommandArr[g_i_CommandNum], g_ach_Operation)==0)
      {
        memset(l_ach_Line, NULL, sizeof(l_ach_Line));
        break;
      }
    }
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Executes desired command

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
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
      static const char l_ach_TimeMsg[] = "Time from program start: ";
      UART_Transmit((uint8_t*)l_ach_TimeMsg, strlen(l_ach_TimeMsg), TRANSMIT_TIMEOUT);
      char l_ach_timeVal[5];
      sprintf(l_ach_timeVal, "%f", TIMER_GetTime());
      UART_Transmit((uint8_t*)l_ach_timeVal, strlen(l_ach_timeVal), TRANSMIT_TIMEOUT);      
      UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);      
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
      sscanf(g_ach_Arg, "%f", &l_fl_ValVolt);
      DAC_Start();
      DAC_SetValue(l_fl_ValVolt);
      static const char l_ach_DacMsg[] = "DAC value: ";
      UART_Transmit((uint8_t*)l_ach_DacMsg, strlen(l_ach_DacMsg), TRANSMIT_TIMEOUT);
      UART_Transmit((uint8_t*)DAC_GetValue(), 1, TRANSMIT_TIMEOUT);
      UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
      DAC_Stop();
      break;
    }
    case EN_CMD_ADC:
    {
      uint32_t l_ui_Timeout;
      sscanf(g_ach_Arg, "%u", &l_ui_Timeout);
      static const char l_ach_AdcMsg[] = "ADC value: ";
      UART_Transmit((uint8_t*)l_ach_AdcMsg,strlen(l_ach_AdcMsg),TRANSMIT_TIMEOUT);
      UART_Transmit((uint8_t*)ADC_GetValue(l_ui_Timeout), 1, TRANSMIT_TIMEOUT);
      UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
      break;
    }
    case EN_CMD_SPI:
    {
      float l_fl_Curr = TIMER_GetTime();
      static const uint32_t l_ui_DelayTime = 1000U;
      static const uint32_t l_ui_NumberOfIteration = 5U;
      L3GD20_Init();
      HAL_Delay(l_ui_DelayTime);
      for(int i = 0; i < l_ui_NumberOfIteration; i++)
      {
        char l_ach_axisValue[5];
        sprintf(l_ach_axisValue, "%f", L3GD20_GetAngularRateX(SENSITIVITY_NONE));
        UART_Transmit((uint8_t*)"X:", 2, TRANSMIT_TIMEOUT);
        UART_Transmit((uint8_t*)l_ach_axisValue, strlen(l_ach_axisValue), TRANSMIT_TIMEOUT);
        UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
        
        sprintf(l_ach_axisValue, "%f", L3GD20_GetAngularRateY(SENSITIVITY_NONE));
        UART_Transmit((uint8_t*)"Y:", 2, TRANSMIT_TIMEOUT);
        UART_Transmit((uint8_t*)l_ach_axisValue, strlen(l_ach_axisValue), TRANSMIT_TIMEOUT);
        UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
        
        sprintf(l_ach_axisValue, "%f", L3GD20_GetAngularRateZ(SENSITIVITY_NONE));
        UART_Transmit((uint8_t*)"Z:", 2, TRANSMIT_TIMEOUT);
        UART_Transmit((uint8_t*)l_ach_axisValue, strlen(l_ach_axisValue), TRANSMIT_TIMEOUT);
        UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
        
        UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
        HAL_Delay(l_ui_DelayTime);
      }
      L3GD20_DeInit();
      break;
    }
    case EN_CMD_PWM:
    {
      static bool g_ui_OnOff = 1U;
      uint32_t l_ui_DutyCycle;
      sscanf(g_ach_Arg, "%u", &l_ui_DutyCycle);
      if(g_ui_OnOff == 1U)
      {
        PWM_Start(l_ui_DutyCycle);
        char l_ach_PwmStartMsg[] = "PWM was started";
        UART_Transmit((uint8_t*)l_ach_PwmStartMsg, strlen(l_ach_PwmStartMsg), TRANSMIT_TIMEOUT);
        g_ui_OnOff=0U;
      }
      else if(g_ui_OnOff == 0U)
      {
        PWM_Stop();
        char l_ach_PwmStopMsg[] = "PWM was stopped";
        UART_Transmit((uint8_t*)l_ach_PwmStopMsg, strlen(l_ach_PwmStopMsg), TRANSMIT_TIMEOUT);
        g_ui_OnOff=1U;
      }
      else
      {
        char l_ach_PwmErrMsg[] = "Something is wrong. Please enter hlp for more details";
        UART_Transmit((uint8_t*)l_ach_PwmErrMsg, strlen(l_ach_PwmErrMsg), TRANSMIT_TIMEOUT);
      }
      UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
      break;
    }
    case EN_CMD_GPIO:
    {
      uint32_t l_ui_PinNumber;
      uint32_t l_ui_RealPin = 1;
      char l_ch_Port;
      sscanf(g_ach_Arg, "%u", &l_ui_PinNumber);
      sscanf(g_ach_Arg2, "%c", &l_ch_Port);
      for(int i = 0; i < l_ui_PinNumber; i++)
      {
        l_ui_RealPin *= 2;
      }

      switch(l_ch_Port)
      {
        case 'a':
        {
          HAL_GPIO_TogglePin(GPIOA, l_ui_RealPin);
          break;
        }
        case 'b':
        {
          HAL_GPIO_TogglePin(GPIOB, l_ui_RealPin);
          break;
        }
        case 'c':
        {
          HAL_GPIO_TogglePin(GPIOC, l_ui_RealPin);
          break;
        }
        case 'd':
        {
          HAL_GPIO_TogglePin(GPIOD, l_ui_RealPin);
          break;
        }
        case 'e':
        {
          HAL_GPIO_TogglePin(GPIOE, l_ui_RealPin);
          break;
        }
        case 'f':
        {
          HAL_GPIO_TogglePin(GPIOF, l_ui_RealPin);
          break;
        }
        case 'g':
        {
          HAL_GPIO_TogglePin(GPIOG, l_ui_RealPin);
          break;
        }
        case 'h':
        {
          HAL_GPIO_TogglePin(GPIOH, l_ui_RealPin);
          break;
        }
        default:
        {
          char l_ach_PortErrMsg[] = "Non-existent port";
          UART_Transmit((uint8_t*)l_ach_PortErrMsg, strlen(l_ach_PortErrMsg), TRANSMIT_TIMEOUT);
          UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
          break;
        }
      }
      if(l_ui_PinNumber > 15)
      {
        char l_ach_PinErrMsg[] = "Pin doesn't exist";
        UART_Transmit((uint8_t*)l_ach_PinErrMsg, strlen(l_ach_PinErrMsg), TRANSMIT_TIMEOUT);
      }
      else
      {
        char l_ach_PinSuccessMsg[] = "Pin has been toggled";
        UART_Transmit((uint8_t*)l_ach_PinSuccessMsg, strlen(l_ach_PinSuccessMsg), TRANSMIT_TIMEOUT);
      }
      UART_Transmit((uint8_t*)g_ach_Newline, strlen(g_ach_Newline), TRANSMIT_TIMEOUT);
      break;
    }
  }
}
