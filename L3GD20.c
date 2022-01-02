#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "L3GD20.h"
#include "spi5.h"

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Configs L3GD20 control registers

 @param         None.

 @return        None. 
*/
/* ----------------------------------------------------------------------------
 */
void L3GD20_Init(void)
{
		SPI5_WriteByte(L3GD20_CTRL_REG1_ADDR,0x0F);
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
void L3GD20_DeInit(void)
{
		SPI5_WriteByte(L3GD20_CTRL_REG1_ADDR,0x07);
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
float L3GD20_GetAngularRateX(float p_fl_Sensitivity)
{
	uint8_t l_ui_XDataH;
	uint8_t l_ui_XDataL;
	int16_t l_i_XDataRaw;

	l_ui_XDataL = SPI5_ReadByte(L3GD20_OUT_X_L_ADDR);
	l_ui_XDataH = SPI5_ReadByte(L3GD20_OUT_X_H_ADDR);
	
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
float L3GD20_GetAngularRateY(float p_fl_Sensitivity)
{
	uint8_t l_ui_YDataH;
	uint8_t l_ui_YDataL;
	int16_t l_i_YDataRaw;

	l_ui_YDataL = SPI5_ReadByte(L3GD20_OUT_Y_L_ADDR);
	l_ui_YDataH = SPI5_ReadByte(L3GD20_OUT_Y_H_ADDR);
	
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
float L3GD20_GetAngularRateZ(float p_fl_Sensitivity)
{
	uint8_t l_ui_ZDataH;
	uint8_t l_ui_ZDataL;
	int16_t l_i_ZDataRaw;

	l_ui_ZDataL = SPI5_ReadByte(L3GD20_OUT_Z_L_ADDR);
	l_ui_ZDataH = SPI5_ReadByte(L3GD20_OUT_Z_H_ADDR);
	
	l_i_ZDataRaw = (int16_t)( (uint16_t)(l_ui_ZDataH << 8) + l_ui_ZDataL);
	return (float)l_i_ZDataRaw/p_fl_Sensitivity;
}
