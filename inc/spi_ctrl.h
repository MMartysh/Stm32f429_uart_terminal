#ifndef SPI_CTRL_H
#define SPI_CTRL_H

void    spiInit(void);
uint8_t spiReadByte(uint8_t address);
void 		spiWriteByte(uint8_t address, uint8_t data);

#endif
