#include "main.h"
#include "uart.h"
#include "string.h"
#include "terminal.h"
#include "pwm.h"
#include "adc.h"
#include "dac.h"
#include <stdlib.h>
#include <stdio.h>

char newline[] = { 0xD,0xA };
char operation[4];
char arg[20];
char arg2[20];
char commandsArr[10][10] =
{
  { "hi" },{ "tim" },{ "hlp" },
	{ "adc" },{ "dac" },{ "spi" },
	{ "pin" },{ "clk" },{ "pwm" }
};
int commandNum = 0;
void aliases()
{
  uartTransmit((uint8_t*)"Command not found, maybe you mean: ", 35, 10);
  for (int i = 0; i<8; i++)
  {
    if (strpbrk(operation, commandsArr[i]) != NULL)
    {
      uartTransmit((uint8_t*)commandsArr[i], strlen(commandsArr[i]), 10);
    }
  }
}
uint16_t toInt(char *num)
{
	uint16_t res=0;
	uint16_t grade = 1;
	for(int i=strlen(num);i>0;i--)
	{
		if(num[i]>47 && num[i]<58)
		{
			res+=(uint16_t)(num[i]-'0')*grade;
		}
		else
		{
			return 0;
		}
		grade*=10;
	}
	return res;
}
void help(void)
{
  for (int i = 0; i<8; i++)
  {
    uartTransmit((uint8_t*)commandsArr[i], 3, 10);
    uartTransmit((uint8_t*)newline, strlen(newline), 10);
  }
}
void tim(void)
{
}
uint32_t dac(void)
{
 switch (toInt(arg))
 {
  case 1:
	{
		return getValue();
	}
  case 0:
	{
		setValue(toInt(arg2));
		break;
	}
 }
 return 1;
}
void pin(void)
{
}
void spi(void)
{
}
void pwm(void)
{
 switch (toInt(arg))
 {
  case 1:
	{
		MX_TIM1_Init(toInt(arg2));
		startPWM();
		break;
	}
  case 0:
	{
		stopPWM();
		break;
	}
 }
}
void clk(void)
{
}
void parse(void)
{
  char line[12];
	int i,y;
  while(uartReceive((uint8_t*)line, 12, 0xffff)!=HAL_OK);
  int cat = 1;
	for(i=0;i<3;i++)
	{
		operation[i] = line[i];
	}
	if((line[3] != ' ' && line[4] != ' ') || (line[3]=='\0'))
	{
		for(y=0;line[i]!='|';i++)
		{
			arg[y]=line[i];
		}
		for(y=0;i<strlen(line);i++)
		{
			arg2[y]=line[i];
		}
	}
	free(line);
  for(commandNum=0;commandNum<8;commandNum++)
  {
		if (strcmp (commandsArr[commandNum], operation)==0)
		{
			break;
		}
  }
  execCommand();
}
int execCommand()
{
  if (commandNum == 0)
  {
    aliases();
  }
  switch (commandNum)
  {
		case 1:tim(); break;
		case 2:
		{
			help();
			break;
		}
		case 3:
		{
			uint8_t a=getValueADC(5);
			uartTransmit(&a,4,10);
			break;
		}
		case 4:
		{
			uint8_t a=dac();
			uartTransmit(&a,4,10);
			break;
		}
	  case 5:spi(); break;
	  case 6:pin(); break;
	  case 7:clk(); break;
	  case 8:pwm(); break;
  }
	if(operation[0])
	{
		free(operation);
	}
	if(arg[0])
	{
		free(arg);
	}
	if(arg2[0])
	{
		free(arg2);
	}
  commandNum = 0;
  return 1;
}