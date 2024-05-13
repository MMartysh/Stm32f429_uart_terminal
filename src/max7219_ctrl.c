#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "spi_ctrl.h"
#include "board.h"
#include "max7219_ctrl.h"
#include "terminal.h"

static uint8_t decodeMode = 0x00;

void max7219Switch(bool isOn)
{
	spiWriteByte(4, REG_SHUTDOWN, (uint8_t)isOn);
}

void max7219SetDecode(bool isOn)
{
	spiWriteByte(4, REG_DECODE_MODE, isOn ? 0xFF : 0x00);
}

void max7219SetIntensity(uint8_t intensity)
{
	if (intensity > 0x0F)
	{
		return;
	}

	spiWriteByte(4, REG_INTENSITY, intensity);
}

void max7219Clear(void)
{
	uint8_t clear = 0x00;

	if(decodeMode == 0xFF)
	{
		clear = 0x0F;
	}

	for (int i = 0; i < 8; ++i)
	{
		spiWriteByte(4, i + 1, clear);
	}
}

void max7219Init(uint8_t intensity)
{
	max7219Switch(true);
	spiWriteByte(4, REG_SCAN_LIMIT, 7);
	max7219SetIntensity(intensity);
	max7219Clear();
}

void max7219Write(uint8_t reg, uint8_t data)
{
   /* Write register */
   spiWriteByte(4, reg, data);
}
void max7219DisplayTest(bool isOn)
{

    /* Write register */
    spiWriteByte(4, REG_SCAN_LIMIT, (uint8_t)isOn);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Set the value on MAX7219 chip pins

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalMax7219SetValue(uint8_t argc, char **argv)
{
    if(argc < 2)
    {
        return false;
    }

    return true;
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Add terminal command handle to list
*/
/* ----------------------------------------------------------------------------
 */
__attribute__((constructor))
void terminalMax7219Init(void)
{
  static commandStruct max7219Command = 
  { 
          .name = "max7219",
          .description = "Control LED matrix. \n",
          .callback = terminalMax7219SetValue,
          .next = NULL 
  };
  terminalAddCommand(&max7219Command);
}