#ifndef TERMINAL_H
#define TERMINAL_H

#define NUMBER_OF_COMMANDS 8  /*Amount of commands*/
#define COMMANDS_LENGTH 5			/*Max length of command*/
#define MAX_ARGUMENT_LENGTH 20 /*Max lenght of argument*/
#define TRANSMIT_TIMEOUT 10 /*Timeout for uart_transmit, 10ms*/
#define RECEIVE_TIMEOUT 100 /*Timeout for uart_receive,100ms*/
#define MAX_STRING_LENGTH 20/*Max lenght of data, that can be gets*/

/**
*function, which run commands
*/

void 				TERMINAL_ExecCommand(void);

/**
*function, which separates string on command name and arguments
*identify command and run it
*/

void 				TERMINAL_Parse(void);

/**
*Initialization terminal(includes initialization all modules)
*/

void 				TERMINAL_Init(void);


/**
*function, which outputs similar commands, if user enter wrong command
*/

static void TERMINAL_Aliases(void);

/**
*function, which output all command
*/

static void TERMINAL_Help(void);

/**
*function, which separates string on command name, argument1, argument2
*/

static void TERMINAL_Echo(void);
#endif
