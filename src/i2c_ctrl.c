#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "i2c_ctrl.h"

#define I2C_COMMUNICATION_TIMEOUT 100

static I2C_HandleTypeDef i2cHandle;

I2C_HandleTypeDef *i2cGetHandle(void)
{
    return &i2cHandle;
}
 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Writes data of arbitrary size via I2C

 @param[in]     address register address to write to.
 @param[in]     data pointer to the buffer with data to write
 @param[in]     size size of the data to write

 @return        True on operation success, false otherwise
*/
/* ----------------------------------------------------------------------------
 */
bool i2cWrite(uint16_t address, uint8_t *data, uint16_t size)
{
    if(HAL_I2C_Master_Transmit(&i2cHandle, address, data, size, I2C_COMMUNICATION_TIMEOUT) != HAL_OK)
    {
        return false;
    }
    return true;
} 


 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Reads data of arbitrary size via I2C

 @param[in]     address device address to read from.
 @param[out]    data pointer to the buffer to store data to
 @param[in]     size size of the data to receive

 @return        True on operation success, false otherwise
*/
/* ----------------------------------------------------------------------------
 */
bool i2cRead(uint16_t address, uint8_t *data, uint16_t size)
{
    if(HAL_I2C_Master_Receive(&i2cHandle, address, data, size, I2C_COMMUNICATION_TIMEOUT) != HAL_OK)
    {
        return false;
    }
    return true;
}

 /* ----------------------------------------------------------------------------
 */
/*!
 @brief         Initializes I2C peripheral
*/
/* ----------------------------------------------------------------------------
 */
void i2cInit(void)
{
    __HAL_RCC_I2C1_CLK_ENABLE();
    i2cHandle.Instance = I2C1;
    i2cHandle.Init.ClockSpeed = 100000;
    i2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2cHandle.Init.OwnAddress1 = 0;
    i2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2cHandle.Init.OwnAddress2 = 0;
    i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&i2cHandle) != HAL_OK)
    {
        Error_Handler();
    }
}


