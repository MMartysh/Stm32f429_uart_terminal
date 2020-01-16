#ifndef TERMINAL_H
#define TERMINAL_H

#define NUMBER_OF_COMMANDS 9
#define COMMANDS_LENGTH 5
#define MAX_ARGUMENT_LENGTH 20
#define TRANSMIT_TIMEOUT 10
#define RECEIVE_TIMEOUT 100
#define MAX_STRING_LENGTH 20

void execCommand(void);
void parse(void);
void terminalInit(void);

#endif
