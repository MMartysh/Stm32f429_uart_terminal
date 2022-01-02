#ifndef SPI_H
#define SPI_H

#define SPI_TIMEOUT 50 /*Spi timeout duration*/
#define SPI_READ 0x80	/*Accelerometer address*/

void SPI5_Init(void);
uint8_t SPI5_ReadByte(uint8_t p_ui_Address);
void 		SPI5_WriteByte(uint8_t p_ui_Address, uint8_t p_ui_Data);

#endif
