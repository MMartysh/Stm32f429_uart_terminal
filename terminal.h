#ifndef TERMINAL_H
#define TERMINAL_H

#define NUMBER_OF_COMMANDS 8
#define COMMANDS_LENGTH 5
#define MAX_ARGUMENT_LENGTH 20
#define TRANSMIT_TIMEOUT 10
#define RECEIVE_TIMEOUT 100
#define MAX_STRING_LENGTH 20

void 				TERMINAL_ExecCommand(void);
void 				TERMINAL_Parse(void);
void 				TERMINAL_Init(void);
static void TERMINAL_Aliases(void);
static void TERMINAL_Help(void);
static void TERMINAL_Echo(void);
#endif
