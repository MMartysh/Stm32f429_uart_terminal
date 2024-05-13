#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "board.h"
#include "terminal.h"
#include "timer_ctrl.h"
#include "spi_ctrl.h"
#include "ili9341_ctrl.h"

#define SDRAM_BASE 0xD0000000
#define SDRAM_SIZE 0x00800000
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define MAKE_RGB(R,G,B) (uint16_t)(((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B & 0x1F))
// Level 1 Commands 
#define LCD_SWRESET             0x01   // Software Reset 
#define LCD_READ_DISPLAY_ID     0x04   // Read display identification information 
#define LCD_RDDST               0x09   // Read Display Status 
#define LCD_RDDPM               0x0A   // Read Display Power Mode 
#define LCD_RDDMADCTL           0x0B   // Read Display MADCTL 
#define LCD_RDDCOLMOD           0x0C   // Read Display Pixel Format 
#define LCD_RDDIM               0x0D   // Read Display Image Format 
#define LCD_RDDSM               0x0E   // Read Display Signal Mode 
#define LCD_RDDSDR              0x0F   // Read Display Self-Diagnostic Result 
#define LCD_SPLIN               0x10   // Enter Sleep Mode 
#define LCD_SLEEP_OUT           0x11   // Sleep out register 
#define LCD_PTLON               0x12   // Partial Mode ON 
#define LCD_NORMAL_MODE_ON      0x13   // Normal Display Mode ON 
#define LCD_DINVOFF             0x20   // Display Inversion OFF 
#define LCD_DINVON              0x21   // Display Inversion ON 
#define LCD_GAMMA               0x26   // Gamma register 
#define LCD_DISPLAY_OFF         0x28   // Display off register 
#define LCD_DISPLAY_ON          0x29   // Display on register 
#define LCD_COLUMN_ADDR         0x2A   // Colomn address register  
#define LCD_PAGE_ADDR           0x2B   // Page address register  
#define LCD_GRAM                0x2C   // GRAM register    
#define LCD_RGBSET              0x2D   // Color SET    
#define LCD_RAMRD               0x2E   // Memory Read    
#define LCD_PLTAR               0x30   // Partial Area    
#define LCD_VSCRDEF             0x33   // Vertical Scrolling Definition    
#define LCD_TEOFF               0x34   // Tearing Effect Line OFF    
#define LCD_TEON                0x35   // Tearing Effect Line ON    
#define LCD_MAC                 0x36   // Memory Access Control register
#define LCD_VSCRSADD            0x37   // Vertical Scrolling Start Address    
#define LCD_IDMOFF              0x38   // Idle Mode OFF    
#define LCD_IDMON               0x39   // Idle Mode ON    
#define LCD_PIXEL_FORMAT        0x3A   // Pixel Format register 
#define LCD_WRITE_MEM_CONTINUE  0x3C   // Write Memory Continue    
#define LCD_READ_MEM_CONTINUE   0x3E   // Read Memory Continue    
#define LCD_SET_TEAR_SCANLINE   0x44   // Set Tear Scanline    
#define LCD_GET_SCANLINE        0x45   // Get Scanline    
#define LCD_WDB                 0x51   // Write Brightness Display register 
#define LCD_RDDISBV             0x52   // Read Display Brightness    
#define LCD_WCD                 0x53   // Write Control Display register
#define LCD_RDCTRLD             0x54   // Read CTRL Display    
#define LCD_WRCABC              0x55   // Write Content Adaptive Brightness Control    
#define LCD_RDCABC              0x56   // Read Content Adaptive Brightness Control    
#define LCD_WRITE_CABC          0x5E   // Write CABC Minimum Brightness    
#define LCD_READ_CABC           0x5F   // Read CABC Minimum Brightness    
#define LCD_READ_ID1            0xDA   // Read ID1 
#define LCD_READ_ID2            0xDB   // Read ID2 
#define LCD_READ_ID3            0xDC   // Read ID3 

// Level 2 Commands 
#define LCD_RGB_INTERFACE       0xB0   // RGB Interface Signal Control 
#define LCD_FRMCTR1             0xB1   // Frame Rate Control (In Normal Mode) 
#define LCD_FRMCTR2             0xB2   // Frame Rate Control (In Idle Mode) 
#define LCD_FRMCTR3             0xB3   // Frame Rate Control (In Partial Mode) 
#define LCD_INVTR               0xB4   // Display Inversion Control 
#define LCD_BPC                 0xB5   // Blanking Porch Control register 
#define LCD_DFC                 0xB6   // Display Function Control register 
#define LCD_ETMOD               0xB7   // Entry Mode Set 
#define LCD_BACKLIGHT1          0xB8   // Backlight Control 1 
#define LCD_BACKLIGHT2          0xB9   // Backlight Control 2 
#define LCD_BACKLIGHT3          0xBA   // Backlight Control 3 
#define LCD_BACKLIGHT4          0xBB   // Backlight Control 4 
#define LCD_BACKLIGHT5          0xBC   // Backlight Control 5 
#define LCD_BACKLIGHT7          0xBE   // Backlight Control 7 
#define LCD_BACKLIGHT8          0xBF   // Backlight Control 8 
#define LCD_POWER1              0xC0   // Power Control 1 register 
#define LCD_POWER2              0xC1   // Power Control 2 register 
#define LCD_VCOM1               0xC5   // VCOM Control 1 register 
#define LCD_VCOM2               0xC7   // VCOM Control 2 register 
#define LCD_NVMWR               0xD0   // NV Memory Write 
#define LCD_NVMPKEY             0xD1   // NV Memory Protection Key 
#define LCD_RDNVM               0xD2   // NV Memory Status Read 
#define LCD_READ_ID4            0xD3   // Read ID4 
#define LCD_PGAMMA              0xE0   // Positive Gamma Correction register 
#define LCD_NGAMMA              0xE1   // Negative Gamma Correction register 
#define LCD_DGAMCTRL1           0xE2   // Digital Gamma Control 1 
#define LCD_DGAMCTRL2           0xE3   // Digital Gamma Control 2 
#define LCD_INTERFACE           0xF6   // Interface control register 

// Extend register commands 
#define LCD_POWERA               0xCB   // Power control A register 
#define LCD_POWERB               0xCF   // Power control B register 
#define LCD_DTCA                 0xE8   // Driver timing control A 
#define LCD_DTCB                 0xEA   // Driver timing control B 
#define LCD_POWER_SEQ            0xED   // Power on sequence register 
#define LCD_3GAMMA_EN            0xF2   // 3 Gamma enable register 
#define LCD_PRC                  0xF7   // Pump ratio control register 
/* Context of text: Cursor location, color and size of text */
txtinfo *ptxinfo;
static const gpioPinCfg dataCmdPin = {GPIOD, {GPIO_PIN_13, GPIO_MODE_AF_PP,      GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}};

static void ili9341ExecuteCommand(uint8_t command)
{
    uint8_t dummy;
    spiTransfer(SPI_CTRL_LCD, &dummy, &command, 1);
}

static void ili9341SendData(uint8_t *data, uint32_t size)
{
    uint8_t dummy[256];
    HAL_GPIO_WritePin(dataCmdPin.gpioPortHandle, dataCmdPin.gpioPinHandle.Pin, GPIO_PIN_RESET);
    spiTransfer(SPI_CTRL_LCD, dummy, data, size);
    HAL_GPIO_WritePin(dataCmdPin.gpioPortHandle, dataCmdPin.gpioPinHandle.Pin, GPIO_PIN_SET);
}

void ili9341DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT))
    {
        *((uint16_t *) (SDRAM_BASE + (x * 2) + (y * (LCD_WIDTH * 2)))) = color;
    }
}

void ili9341DrawCircle(uint16_t xc, uint16_t yc, uint16_t radius, uint16_t color)
{
    for(int16_t y1 =- radius; y1 <= radius; y1++)
    {
        for(int16_t x1 =- radius; x1 <= radius; x1++)
        {
            if(x1*x1+y1*y1 <= radius*radius)
            {
                ili9341DrawPixel(x1 + xc, y1 + yc, color);
            }
        }
    }
}

void ili9341Init(void)
{
	HAL_GPIO_WritePin(dataCmdPin.gpioPortHandle, dataCmdPin.gpioPinHandle.Pin, GPIO_PIN_SET);
    //Frame Rate Control (In Normal Mode)
	ili9341ExecuteCommand(LCD_FRMCTR1); 
    uint8_t commandByte = 0x00;
    ili9341SendData(&commandByte, 1);
    commandByte = 0x18;
    ili9341SendData(&commandByte, 1);
    
    //Power Control 1
	ili9341ExecuteCommand(LCD_POWER1); 
    commandByte = 0x10;
    ili9341SendData(&commandByte, 1);

    //Power Control 2
	ili9341ExecuteCommand(LCD_POWER2); 
    commandByte = 0x10; //useless, but needed if power control 1 setting will change
    ili9341SendData(&commandByte, 1);	
    
    //VCOM Control 1 
	ili9341ExecuteCommand(LCD_VCOM1); 	
    commandByte = 0x45;
    ili9341SendData(&commandByte, 1);
    commandByte = 0x15;
    ili9341SendData(&commandByte, 1);
    
    //VCOM Control 7 
	ili9341ExecuteCommand(LCD_VCOM2); 
    commandByte = 0x90;
    ili9341SendData(&commandByte, 1);	
    
    //Memory Access Control register
	ili9341ExecuteCommand(LCD_MAC); 	
    commandByte = 0xc8;
	ili9341SendData(&commandByte, 1);		//With 0x08 turns upside down 
    
    //RGB Interface Signal	
	ili9341ExecuteCommand(LCD_RGB_INTERFACE);
    commandByte = 0xc2;
    ili9341SendData(&commandByte, 1);	
	
    //Pixel Format: 16 bit/pix 
	ili9341ExecuteCommand(LCD_PIXEL_FORMAT);
    commandByte = 0x55;
    ili9341SendData(&commandByte, 1);	
    
    //Display Function 
	ili9341ExecuteCommand(LCD_DFC);
    commandByte = 0x0A;
    ili9341SendData(&commandByte, 1);
    commandByte = 0xA7;
    ili9341SendData(&commandByte, 1);
    commandByte = 0x27;
    ili9341SendData(&commandByte, 1);
    commandByte = 0x04;
    ili9341SendData(&commandByte, 1);;	
    
    //Gamma register 
	timerDelayMs(200);
	ili9341ExecuteCommand(LCD_GAMMA);
    commandByte = 0x01;
    ili9341SendData(&commandByte, 1);
    
    
    static uint8_t pos_gamma_args[] = 
    { 
        0x0F, 0x29, 0x24, 0x0C, 0x0E, 
        0x09, 0x4E, 0x78, 0x3C, 0x09, 
        0x13, 0x05, 0x17, 0x11, 0x00
    } ;
	
    static uint8_t neg_gamma_args[] = 
    { 
        0x00, 0x16, 0x1B, 0x04, 0x11, 
        0x07, 0x31, 0x33, 0x42, 0x05, 
        0x0C, 0x0A, 0x28, 0x2F, 0x0F 
    };
    
    //Positive Gamma Correction 
	ili9341ExecuteCommand(LCD_PGAMMA);	
	ili9341SendData(pos_gamma_args, ((sizeof(pos_gamma_args) / sizeof(pos_gamma_args[0])) - 1));

    //Negative Gamma Correction 
	ili9341ExecuteCommand(LCD_NGAMMA); 	
	ili9341SendData(neg_gamma_args, ((sizeof(neg_gamma_args) / sizeof(neg_gamma_args[0])) - 1));

	ili9341ExecuteCommand(LCD_SLEEP_OUT);
	timerDelayMs(200);
	
    ili9341ExecuteCommand(LCD_DISPLAY_ON);
	timerDelayMs(20);
    
    ili9341DrawCircle(20, 100, 20, MAKE_RGB(0,0,0));
}
