#ifndef SPI_CTRL_H
#define SPI_CTRL_H

#define SPI_TIMEOUT 50 /*Spi timeout duration*/
#define SPI_READ 0x80	/*Accelerometer address*/

void    spiInit(void);
uint8_t spiReadByte(uint8_t p_ui_Address);
void 		spiWriteByte(uint8_t p_ui_Address, uint8_t p_ui_Data);

#endif
