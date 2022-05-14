#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "l3gd20_ctrl.h"
#include "spi_ctrl.h"
#include "terminal.h"

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Configs L3GD20 control registers

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void l3gd20Init(void)
{
    spiWriteByte(L3GD20_CTRL_REG1_ADDR, 0x0F);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Deinitializes L3GD20

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void l3gd20DeInit(void)
{
    spiWriteByte(L3GD20_CTRL_REG1_ADDR, 0x07);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Deinitializes L3GD20

 @param[in]     axisNum X, Y or Z axis, can be: 
                    L3GD20_X_AXIS,
                    L3GD20_Y_AXIS,
                    L3GD20_Z_AXIS
 @param[in]     isHighNumberPart is H or L register

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
static uint8_t l3gd20GetAxis(uint8_t axisNum, bool isHighNumberPart)
{
    switch(axisNum)
    {
        case L3GD20_X_AXIS:
            return isHighNumberPart ? L3GD20_OUT_X_H_ADDR : L3GD20_OUT_X_L_ADDR;

        case L3GD20_Y_AXIS:
            return isHighNumberPart ? L3GD20_OUT_Y_H_ADDR : L3GD20_OUT_Y_L_ADDR;

        case L3GD20_Z_AXIS:
            return isHighNumberPart ? L3GD20_OUT_Z_H_ADDR : L3GD20_OUT_Z_L_ADDR;

        default:
            return L3GD20_OUT_Z_H_ADDR;

    }
}
/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Returns X axis acceleration rate

 @param[in]     sensitivity accelerometer sensitivity.

 @return        Angle acceleration rates. 
*/
/* ----------------------------------------------------------------------------
 */
float l3gd20GetAxisAngularRate(uint8_t axisNum, float sensitivity)
{
	uint8_t axisDataH;
	uint8_t axisDataL;
	int16_t regRawData;

	axisDataL = spiReadByte(l3gd20GetAxis(axisNum, false));
	axisDataH = spiReadByte(l3gd20GetAxis(axisNum, true));
	
	regRawData = (int16_t)( (uint16_t)(axisDataH << 8) + axisDataL);
	return (float)(regRawData / sensitivity);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Return ADC value

 @param         None.

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalAccelerometerGetValue(uint8_t argc, char **argv)
{
    (void)argc;
    static const uint32_t delayTime = 1000U;
    l3gd20Init();
    HAL_Delay(delayTime);
    //reading values five times
    for(int i = 0; i < 5; i++)
    {
        printf("X axis: %f\n", l3gd20GetAxisAngularRate(L3GD20_X_AXIS, SENSITIVITY_NONE));
        printf("Y axis: %f\n", l3gd20GetAxisAngularRate(L3GD20_Y_AXIS, SENSITIVITY_NONE));
        printf("Z axis: %f\n", l3gd20GetAxisAngularRate(L3GD20_X_AXIS, SENSITIVITY_NONE));
        HAL_Delay(delayTime);
    }
    l3gd20DeInit();
    return true;
}

__attribute__((constructor))
void terminalAccInit(void)
{
    static commandStruct accCommand = 
    { 
        .name = "acc",
        .description = "Returns XYZ axices acceleration 5 times. \nacc",
        .callback = terminalAccelerometerGetValue,
        .next = NULL 
    };
    terminalAddCommand(&accCommand);
}