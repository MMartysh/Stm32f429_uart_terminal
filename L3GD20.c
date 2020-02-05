#include "L3GD20.h"
#include "spi5.h"

/*
*Initialization L3GD20
*/

void L3GD20_Init(void)
{
		SPI5_Write(L3GD20_CTRL_REG1_ADDR,0x0F);
}

/*
*Deinitialization L3GD20
*/

void L3GD20_DeInit(void)
{
		SPI5_Write(L3GD20_CTRL_REG1_ADDR,0x07);
}

/**
 *function which returns X rotation
 *@param {float} p_fl_Sensitivity
 *@return {float}
 */
 

float L3GD20_GetAngularRateX(float p_fl_Sensitivity)
{
	float 	l_fl_angle;
	uint8_t l_ui_XDataH;
	uint8_t l_ui_XDataL;
	int16_t l_i_XDataRaw;

	l_ui_XDataL=SPI5_Read(L3GD20_OUT_X_L_ADDR);
	l_ui_XDataH=SPI5_Read(L3GD20_OUT_X_H_ADDR);
	
	l_i_XDataRaw = (int16_t)( (uint16_t)(l_ui_XDataH << 8) + l_ui_XDataL);
	l_fl_angle = (float)l_i_XDataRaw/p_fl_Sensitivity;
	return l_fl_angle;
}


/**
 *function which returns Y rotation
 *@param {float} p_fl_Sensitivity
 *@return {float}
 */
 

float L3GD20_GetAngularRateY(float p_fl_Sensitivity)
{
	float l_fl_angle;
	uint8_t l_ui_YDataH;
	uint8_t l_ui_YDataL;
	int16_t l_i_YDataRaw;

	l_ui_YDataL=SPI5_Read(L3GD20_OUT_Y_L_ADDR);
	l_ui_YDataH=SPI5_Read(L3GD20_OUT_Y_H_ADDR);
	
	l_i_YDataRaw = (int16_t)( (uint16_t)(l_ui_YDataH << 8) + l_ui_YDataL);
	l_fl_angle = (float)l_i_YDataRaw/p_fl_Sensitivity;
	return l_fl_angle;
}


/**
 *function which returns Z rotation
 *@param {float} p_fl_Sensitivity
 *@return {float}
 */
 
float L3GD20_GetAngularRateZ(float p_fl_Sensitivity)
{
	float l_fl_angle;
	uint8_t l_ui_ZDataH;
	uint8_t l_ui_ZDataL;
	int16_t l_i_ZDataRaw;

	l_ui_ZDataL=SPI5_Read(L3GD20_OUT_Z_L_ADDR);
	l_ui_ZDataH=SPI5_Read(L3GD20_OUT_Z_H_ADDR);
	
	l_i_ZDataRaw = (int16_t)( (uint16_t)(l_ui_ZDataH << 8) + l_ui_ZDataL);
	l_fl_angle = (float)l_i_ZDataRaw/p_fl_Sensitivity;
	return l_fl_angle;
}
