#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "lcd1602a_ctrl.h"
#include "board.h"
#include "terminal.h"

gpioPinCfg lcd1602Pins[] = 
{
    {GPIOD, {GPIO_PIN_9,  GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D0
    {GPIOD, {GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D1
    {GPIOD, {GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D2
    {GPIOD, {GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D3
    {GPIOD, {GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D4
    {GPIOD, {GPIO_PIN_14, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D5
    {GPIOD, {GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D6
    {GPIOD, {GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_D7
    {GPIOG, {GPIO_PIN_2,  GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_RW
    {GPIOG, {GPIO_PIN_3,  GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},    //LCD1602A_RS
    
};
static void lcd1602ExecuteCommand(lcd1602CommandBits *command)
{
    for(uint8_t i = 0; i < sizeof(command->value); i++)
    {
        HAL_GPIO_WritePin(lcd1602Pins[i].gpioPortHandle,
                          lcd1602Pins[i].gpioPinHandle.Pin,
        (command->value & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}
void lcd1602Init(void)
{
    //set the function for the LCD
    //currently using the example from datasheet
    lcd1602CommandBits currentCommand = 
    {
        .functionSet = 
        {
            .reserved1    = 0,
            .font         = 0,
            .displayLines = 1,
            .dataLength   = 1,
            .putOneHere   = 1, //command flag
            .reserved2    = 0,
        }
    };
    lcd1602ExecuteCommand(&currentCommand);

    currentCommand.displaySettings.displayOnOff    = 0;
    currentCommand.displaySettings.cursorOnOff     = 1;
    currentCommand.displaySettings.cursorBlinking  = 1;
    currentCommand.displaySettings.reserved        = 0;
    currentCommand.displaySettings.putOneHere      = 1;
    lcd1602ExecuteCommand(&currentCommand);

    currentCommand.entryMode.moveDir         = 0;
    currentCommand.entryMode.displayShift    = 1;
    currentCommand.entryMode.putOneHere      = 1;
    currentCommand.entryMode.reserved        = 0;
    lcd1602ExecuteCommand(&currentCommand);
}

void lcd1602WriteSymbol(uint8_t address, uint8_t symbol)
{
    lcd1602CommandBits writeCommand =
    {
        .setCgramAddr = 
        {
            .addr = address,
            .putOneHere = 1,
            .reserved = 0
        }
    };        
    lcd1602ExecuteCommand(&writeCommand);
    writeCommand.value = 0;
    writeCommand.displaySymbol.symbol = symbol;
    writeCommand.displaySymbol.putOneHere = 1;
    lcd1602ExecuteCommand(&writeCommand);
}