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
		spiWriteByte(L3GD20_CTRL_REG1_ADDR,0x0F);
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
		spiWriteByte(L3GD20_CTRL_REG1_ADDR,0x07);
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Returns X axis acceleration rate

 @param[in]     p_fl_Sensitivity accelerometer sensitivity.

 @return        Angle acceleration rates. 
*/
/* ----------------------------------------------------------------------------
 */
float l3gd20GetAngularRateX(float p_fl_Sensitivity)
{
	uint8_t l_ui_XDataH;
	uint8_t l_ui_XDataL;
	int16_t l_i_XDataRaw;

	l_ui_XDataL = spiReadByte(L3GD20_OUT_X_L_ADDR);
	l_ui_XDataH = spiReadByte(L3GD20_OUT_X_H_ADDR);
	
	l_i_XDataRaw = (int16_t)( (uint16_t)(l_ui_XDataH << 8) + l_ui_XDataL);
	return (float)(l_i_XDataRaw / p_fl_Sensitivity);
}


/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Returns Y axis acceleration rate

 @param[in]     p_fl_Sensitivity accelerometer sensitivity.

 @return        Angle acceleration rates. 
*/
/* ----------------------------------------------------------------------------
 */
float l3gd20GetAngularRateY(float p_fl_Sensitivity)
{
	uint8_t l_ui_YDataH;
	uint8_t l_ui_YDataL;
	int16_t l_i_YDataRaw;

	l_ui_YDataL = spiReadByte(L3GD20_OUT_Y_L_ADDR);
	l_ui_YDataH = spiReadByte(L3GD20_OUT_Y_H_ADDR);
	
	l_i_YDataRaw = (int16_t)( (uint16_t)(l_ui_YDataH << 8) + l_ui_YDataL);
	return (float)(l_i_YDataRaw / p_fl_Sensitivity);
}


/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Returns Z axis acceleration rate

 @param[in]     p_fl_Sensitivity accelerometer sensitivity.

 @return        Angle acceleration rates. 
*/
/* ----------------------------------------------------------------------------
 */
float l3gd20GetAngularRateZ(float p_fl_Sensitivity)
{
	uint8_t l_ui_ZDataH;
	uint8_t l_ui_ZDataL;
	int16_t l_i_ZDataRaw;

	l_ui_ZDataL = spiReadByte(L3GD20_OUT_Z_L_ADDR);
	l_ui_ZDataH = spiReadByte(L3GD20_OUT_Z_H_ADDR);
	
	l_i_ZDataRaw = (int16_t)( (uint16_t)(l_ui_ZDataH << 8) + l_ui_ZDataL);
	return (float)l_i_ZDataRaw/p_fl_Sensitivity;
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
    printf("X axis: %f\n", l3gd20GetAngularRateX(SENSITIVITY_NONE));
    printf("Y axis: %f\n", l3gd20GetAngularRateY(SENSITIVITY_NONE));
    printf("Z axis: %f\n", l3gd20GetAngularRateZ(SENSITIVITY_NONE));
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