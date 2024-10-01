#ifndef SPI_CTRL_H
#define SPI_CTRL_H

typedef enum csDeviceName
{
    SPI_CTRL_MAX7219,
    SPI_CTRL_ACC,
    SPI_CTRL_LCD,
    SPI_CTRL_DEV_MAX_NUM
}csDeviceName;
HAL_StatusTypeDef spiInit(uint8_t devNum);
HAL_StatusTypeDef spiReadByte(uint8_t devNum, uint8_t address, uint8_t *data);
HAL_StatusTypeDef spiWriteByte(uint8_t devNum, uint8_t address, uint8_t data);
HAL_StatusTypeDef spiTransfer(uint8_t devNum, uint8_t *rxData, uint8_t *txData, uint32_t size);

#endif
