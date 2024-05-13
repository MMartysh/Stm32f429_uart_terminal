#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "timer_ctrl.h"
#include "lcd1602a_ctrl.h"
#include "board.h"
#include "terminal.h"
lcdHandleStr lcd;
static bool lcdSend(lcdHandleStr *lcdI2c, uint8_t cmd);
static void lcdDisplay(lcdHandleStr *lcdI2c, uint8_t data);

void lcdInit(lcdHandleStr *lcdI2c, I2C_HandleTypeDef *handle, uint8_t addr, uint8_t cols, uint8_t rows)
{
    lcdI2c->i2c = handle;

	lcdI2c->address = (addr << 1); //correct address
  	lcdI2c->columns = cols;
  	lcdI2c->rows = rows;
	lcdI2c->lines = rows;
  	lcdI2c->backlight = LCD_NOBACKLIGHT;
	lcdI2c->displayControl = LCD_CURSOROFF | LCD_BLINKOFF;
	
	// 4 bit initialisation
	HAL_Delay(50); // wait for >40ms
	lcdSend(lcdI2c, 0x30);
	HAL_Delay(5); // wait for >4.1ms
	lcdSend(lcdI2c, 0x30);
	HAL_Delay(1); // wait for >100us
	lcdSend(lcdI2c, 0x30);
	HAL_Delay(10);
	lcdSend(lcdI2c, 0x20); // 4bit mode
	HAL_Delay(10);

	// display initialisation
	lcdSend(lcdI2c, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcdSend(lcdI2c, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcdSend(lcdI2c, 0x01); // clear display
	HAL_Delay(2);
	lcdSend(lcdI2c, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcdSend(lcdI2c, 0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

}

void lcdHome(lcdHandleStr *lcdI2c)
{
	lcdSend(lcdI2c, LCD_RETURNHOME);
	timerDelayMs(2);
}

void lcdClear(lcdHandleStr *lcdI2c)
{
	lcdSend(lcdI2c, LCD_CLEARDISPLAY);
	timerDelayMs(2);
}

void lcdBacklight(lcdHandleStr *lcdI2c, bool backlight)
{
	if (backlight) 
    {
        lcdI2c->backlight = LCD_BACKLIGHT;	
    }
	else 
    {
        lcdI2c->backlight = 0;
    }
	lcdSend(lcdI2c, 0);
}

void lcdBlink(lcdHandleStr *lcd_i2c, bool on)
{
	if(on) 
    {
		lcd_i2c->displayControl |= LCD_BLINKON;
	} 
    else 
    {
		lcd_i2c->displayControl &= ~LCD_BLINKON;
	}
	lcdSend(lcd_i2c, LCD_DISPLAYCONTROL | LCD_DISPLAYON | lcd_i2c->displayControl);
}

void lcdSetCursor(lcdHandleStr *lcdI2c, bool cur)
{
	if(cur) 
    {
		lcdI2c->displayControl |= LCD_CURSORON;
	} 
    else 
    {
		lcdI2c->displayControl &= ~LCD_CURSORON;
	}
	lcdSend(lcdI2c, LCD_DISPLAYCONTROL | LCD_DISPLAYON | lcdI2c->displayControl);
}

void lcdSetCursorPos(lcdHandleStr *lcdI2c, uint8_t col, uint8_t row)
{
	if((row + 1) > lcdI2c->rows) return;

	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if( row > (4 - 1) ) 
    {
		row = 4 - 1; 
	}
	lcdSend(lcdI2c, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcdSendString(lcdHandleStr *lcdI2c, const uint8_t *str, uint8_t size)
{
    if(str == NULL || size == 0)
    {
        return;
    }
    for(uint8_t idx = 0; idx < size; idx++)
    {
        lcdDisplay(lcdI2c, str[idx]);
    }
}

bool lcdSend(lcdHandleStr *lcdI2c, uint8_t cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C; //en=1, rs=0
	data_t[1] = data_u | 0x08; //en=0, rs=0
	data_t[2] = data_l | 0x0C; //en=1, rs=0
	data_t[3] = data_l | 0x08; //en=0, rs=0
	HAL_I2C_Master_Transmit(lcdI2c->i2c, lcdI2c->address, (uint8_t *)data_t, 4, 100);
    return true;
}

void lcdDisplay(lcdHandleStr *lcdI2c, uint8_t data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D; //en=1, rs=1
	data_t[1] = data_u | 0x09; //en=0, rs=1
	data_t[2] = data_l | 0x0D; //en=1, rs=1
	data_t[3] = data_l | 0x09; //en=0, rs=1
	HAL_I2C_Master_Transmit(lcdI2c->i2c, lcdI2c->address, (uint8_t *)data_t, 4, 100);
}

bool terminalLcdWrite(uint8_t argc, char **argv)
{
    if(argc < 2)
    {
        return false;
    }
    for(uint8_t strIdx = 1; strIdx < argc; strIdx++)
    {
        lcdSendString(&lcd, (uint8_t*)argv[strIdx], strlen(argv[strIdx]));
        lcdDisplay(&lcd, 32);
    }
    return true;
}

__attribute__((constructor))
void terminalLcdInit(void)
{
  static commandStruct lcdCommand = 
  { 
          .name = "lcd",
          .description = "Display string on LCD. \nlcd [arg1]..[argn] \n arg1 - argn string to display ",
          .callback = terminalLcdWrite,
          .next = NULL 
  };
  terminalAddCommand(&lcdCommand);
}
