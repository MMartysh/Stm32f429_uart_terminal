#ifndef L3GD20_CTRL_H
#define L3GD20_CTRL_H

#define L3GD20_WHO_AM_I_ADDR          0x0F  /* device identification register */
#define SENSITIVITY_NONE 								1		/*Sensitivity off*/
#define SENSITIVITY_250 							114.285 /*Level sensitivity*/
typedef enum l3gd20Regs
{
  L3GD20_CTRL_REG1_ADDR     = 0x20, /* Control register 1 */
  L3GD20_CTRL_REG2_ADDR     = 0x21, /* Control register 2 */
  L3GD20_CTRL_REG3_ADDR     = 0x22, /* Control register 3 */
  L3GD20_CTRL_REG4_ADDR     = 0x23, /* Control register 4 */
  L3GD20_CTRL_REG5_ADDR     = 0x24, /* Control register 5 */
  L3GD20_REFERENCE_REG_ADDR = 0x25, /* Reference register */
  L3GD20_OUT_TEMP_ADDR      = 0x26, /* Out temp register */
  L3GD20_STATUS_REG_ADDR    = 0x27, /* Status register */
  L3GD20_OUT_X_L_ADDR       = 0x28, /* Output Register X */
  L3GD20_OUT_X_H_ADDR       = 0x29, /* Output Register X */
  L3GD20_OUT_Y_L_ADDR       = 0x2A, /* Output Register Y */
  L3GD20_OUT_Y_H_ADDR       = 0x2B, /* Output Register Y */
  L3GD20_OUT_Z_L_ADDR       = 0x2C, /* Output Register Z */
  L3GD20_OUT_Z_H_ADDR       = 0x2D, /* Output Register Z */
  L3GD20_FIFO_CTRL_REG_ADDR = 0x2E, /* Fifo control Register */
  L3GD20_FIFO_SRC_REG_ADDR  = 0x2F, /* Fifo src Register */                                  
  L3GD20_INT1_CFG_ADDR      = 0x30, /* Interrupt 1 configuration Register */
  L3GD20_INT1_SRC_ADDR      = 0x31, /* Interrupt 1 source Register */
  L3GD20_INT1_TSH_XH_ADDR   = 0x32, /* Interrupt 1 Threshold X register */
  L3GD20_INT1_TSH_XL_ADDR   = 0x33, /* Interrupt 1 Threshold X register */
  L3GD20_INT1_TSH_YH_ADDR   = 0x34, /* Interrupt 1 Threshold Y register */
  L3GD20_INT1_TSH_YL_ADDR   = 0x35, /* Interrupt 1 Threshold Y register */
  L3GD20_INT1_TSH_ZH_ADDR   = 0x36, /* Interrupt 1 Threshold Z register */
  L3GD20_INT1_TSH_ZL_ADDR   = 0x37, /* Interrupt 1 Threshold Z register */
  L3GD20_INT1_DURATION_ADDR = 0x38, /* Interrupt 1 DURATION register */
}l3gd20Regs;
typedef enum l3gd20Axis
{
  L3GD20_X_AXIS,
  L3GD20_Y_AXIS,
  L3GD20_Z_AXIS
}l3gd20Axis;
float l3gd20GetAxisAngularRate(uint8_t axisNum, float sensitivity);
void 	l3gd20Init(void);
void 	l3gd20DeInit(void);

#endif
