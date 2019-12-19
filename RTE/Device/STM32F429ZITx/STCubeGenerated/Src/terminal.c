#include "main.h"
#include "uart.h"
#include "string.h"


void parse(void){
	
	char Data[30]="Hello\r\t";
	char getData[10];
	uartTransmit((uint8_t*)Data, strlen(Data),10);
	uartReceive((uint8_t*)getData, 10, 0xffff);
	uartTransmit((uint8_t*)getData, strlen(getData),10);
	//HAL_UART_Receive(&huart1,(uint8_t*)getData, 10,0xffff);
	//uartTransmit((uint8_t*)getData, strlen(getData),10);
}