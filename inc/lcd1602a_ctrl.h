#ifndef LCD_CTRL_H
#define LCD_CTRL_H

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define BIT_EN 0b00000100  // Enable bit
#define BIT_RW 0b00000010  // Read/Write bit
#define BIT_RS 0b00000001  // Register select bit

typedef struct 
{
    I2C_HandleTypeDef *i2c;
    uint8_t address;
    uint8_t displayControl;
    uint8_t displayMode;
    uint8_t backlight;
    uint8_t lines;
    uint8_t columns;
    uint8_t rows;
} lcdHandleStr;
extern lcdHandleStr lcd;
void lcdInit(lcdHandleStr *lcdI2c, I2C_HandleTypeDef *handle, uint8_t addr, uint8_t cols, uint8_t rows);
void lcdHome(lcdHandleStr *lcdI2c);
void lcdClear(lcdHandleStr *lcdI2c);
void lcdBacklight(lcdHandleStr *lcdI2c, bool backlight);
void lcdBlink(lcdHandleStr *lcd_i2c, bool on);
void lcdSetCursor(lcdHandleStr *lcdI2c, bool cur);
void lcdSetCursorPos(lcdHandleStr *lcdI2c, uint8_t col, uint8_t row);
void lcdSendString(lcdHandleStr *lcdI2c, const uint8_t *str, uint8_t size);
#endif
