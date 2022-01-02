#ifndef TERMINAL_H
#define TERMINAL_H

#define NUMBER_OF_COMMANDS 8  /*Amount of commands*/
#define COMMANDS_LENGTH 5			/*Max length of command*/
#define MAX_ARGUMENT_LENGTH 20 /*Max lenght of argument*/
#define TRANSMIT_TIMEOUT 10 /*Timeout for uart_transmit, 10ms*/
#define RECEIVE_TIMEOUT 100 /*Timeout for uart_receive,100ms*/
#define MAX_STRING_LENGTH 20/*Max lenght of data, that can be gets*/


void  TERMINAL_ExecCommand(void);
void  TERMINAL_Parse(void);
void  TERMINAL_Init(void);

#endif
