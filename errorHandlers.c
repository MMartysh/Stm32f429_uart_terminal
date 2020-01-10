#include "errorHandlers.h"


void Error_Handler(void)
{
	LED_Initialize();
	LED_On(0);
	while(1)
	{
	}
}
/*void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}*/


