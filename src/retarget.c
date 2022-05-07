#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "uart_ctrl.h"

FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f)
{
  (void)f;
  if (c == '\n')
  {
    (void)uartSendChar('\r');
  }
  return uartSendChar((char)c);
}

int fgetc(FILE *f)
{
  (void)f;
  return uartGetChar();
}

int ferror(FILE *f)
{
  (void)f;
  return EOF;
}

void _ttywrch(int c)
{
  (void)uartSendChar((char)c);
}
