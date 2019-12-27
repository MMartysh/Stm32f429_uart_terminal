#ifndef TERMINAL_H
#define TERMINAL_H

#define NUMBER_OF_COMMANDS 8
#define COMMANDS_LENGTH 5
#define MAX_ARGUMENT_LENGTH 20
#define TRANSMIT_TIMEOUT 10

void execCommand(void);
void parse(void);
void terminalInit(void);

#endif
