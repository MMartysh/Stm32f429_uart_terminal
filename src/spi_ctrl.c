#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "board.h"
#include "spi_ctrl.h"
#define SPI_NUMBER  6       /*Number of available SPI*/
#define SPI_TIMEOUT 50      /*Spi timeout duration*/
#define SPI_READ    0x80	/*SPI read flag*/
//spi handler
static struct
{
    SPI_HandleTypeDef handler;
    GPIO_TypeDef *gpioPortHandle;
    uint32_t gpioPinNumber;
}
spiCtrlData[] = 
{
    {{.Instance = SPI4}, GPIOE, GPIO_PIN_11},
    {{.Instance = SPI5}, GPIOC, GPIO_PIN_1 },
    {{.Instance = SPI5}, GPIOC, GPIO_PIN_2 }
};
 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Writes 1 byte of data via SPI

 @param[in]     devNum device number, should be of type csDeviceName.
 @param[in]     address register address to write to.
 @param[in]     data data to be written.

 @return        Status of operation. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef spiWriteByte(uint8_t devNum, uint8_t address, uint8_t data)
{
    if(devNum > SPI_CTRL_DEV_MAX_NUM)
    {
        return HAL_ERROR;
    }
    uint8_t txVal[] = {address, data};
    HAL_GPIO_WritePin(spiCtrlData[devNum].gpioPortHandle, spiCtrlData[devNum].gpioPinNumber, GPIO_PIN_RESET); //CS --> Low
    HAL_StatusTypeDef retVal = HAL_SPI_Transmit(&spiCtrlData[devNum].handler, txVal, 2, SPI_TIMEOUT);
    HAL_GPIO_WritePin(spiCtrlData[devNum].gpioPortHandle, spiCtrlData[devNum].gpioPinNumber, GPIO_PIN_SET); //CS --> High
    return retVal;
} 


 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Reads 1 byte of data via SPI

 @param[in]     devNum device number, should be of type csDeviceName.
 @param[in]     address register address to read from.
 @param[out]    data data to be read.

 @return        Status of operation. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef spiReadByte(uint8_t devNum, uint8_t address, uint8_t *data)
{
    if(devNum > SPI_CTRL_DEV_MAX_NUM)
    {
        return HAL_ERROR;
    }
	address = address | SPI_READ;
    HAL_GPIO_WritePin(spiCtrlData[devNum].gpioPortHandle, spiCtrlData[devNum].gpioPinNumber, GPIO_PIN_RESET); //CS --> Low
    HAL_StatusTypeDef retVal = HAL_SPI_TransmitReceive(&spiCtrlData[devNum].handler, &address, data, 1, SPI_TIMEOUT);
    HAL_GPIO_WritePin(spiCtrlData[devNum].gpioPortHandle, spiCtrlData[devNum].gpioPinNumber, GPIO_PIN_SET); //CS --> High
    return retVal;
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Perform SPI data transfer

 @param[in]     devNum device number, should be of type csDeviceName.
 @param[out]    rxData vuffer to store received data to.
 @param[in]     txData data to be transferred.
 @param[in]     size transfer size.

 @return        Status of operation. 
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef spiTransfer(uint8_t devNum, uint8_t *rxData, uint8_t *txData, uint32_t size)
{
    if(devNum > SPI_CTRL_DEV_MAX_NUM)
    {
        return HAL_ERROR;
    }
    HAL_GPIO_WritePin(spiCtrlData[devNum].gpioPortHandle, spiCtrlData[devNum].gpioPinNumber, GPIO_PIN_RESET); //CS --> Low
    HAL_StatusTypeDef retVal = HAL_SPI_TransmitReceive(&spiCtrlData[devNum].handler, txData, rxData, size, SPI_TIMEOUT);
    HAL_GPIO_WritePin(spiCtrlData[devNum].gpioPortHandle, spiCtrlData[devNum].gpioPinNumber, GPIO_PIN_SET); //CS --> High
    return retVal;
}
 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes SPI peripheral
*/
/* ----------------------------------------------------------------------------
 */
HAL_StatusTypeDef spiInit(uint8_t devNum)
{
    if(devNum > SPI_CTRL_DEV_MAX_NUM)
    {
        return HAL_ERROR;
    }
    SPI_InitTypeDef spiInit = 
    {
        .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16, //SPI_BAUDRATEPRESCALER_8 for MAX7219?
        .Direction         = SPI_DIRECTION_2LINES,
        .CLKPhase          = SPI_PHASE_1EDGE,
        .CLKPolarity       = SPI_POLARITY_LOW,
        .DataSize          = SPI_DATASIZE_8BIT,
        .FirstBit          = SPI_FIRSTBIT_MSB,
        .TIMode            = SPI_TIMODE_DISABLE,
        .CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
        .CRCPolynomial     = 10,
        .NSS               = SPI_NSS_SOFT,
        .Mode              = SPI_MODE_MASTER,
    };
    spiCtrlData[devNum].handler.Init = spiInit;
    return HAL_SPI_Init(&spiCtrlData[devNum].handler);
}


