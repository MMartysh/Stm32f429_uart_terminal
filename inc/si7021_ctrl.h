#ifndef SI7021_CTRL_H
#define SI7021_CTRL_H

#define SI7021_RHT_USER_REG_D0  1 
#define SI7021_RHT_USER_REG_D2 (1 << 2)
#define SI7021_RHT_USER_REG_D6 (1 << 6)
#define SI7021_RHT_USER_REG_D7 (1 << 7)
typedef enum si7021Resolution
{
  SI7021_RES_12_RH_14_TEMP,
  SI7021_RES_08_RH_12_TEMP,
  SI7021_RES_10_RH_13_TEMP,
  SI7021_RES_11_RH_11_TEMP
}si7021Resolution;

typedef enum si7021Operations
{
  SI7021_HUMIDITY_MASTER_HOLD        = 0xE5, // Measure Relative Humidity, Hold Master Mode
  SI7021_HUMIDITY_MASTER_NO_HOLD     = 0xF5, // Measure Relative Humidity, No Hold Master Mode
  SI7021_TEMPERATURE_MASTER_HOLD     = 0xE3, // Measure Temperature, Hold Master Mode
  SI7021_TEMPERATURE_MASTER_NO_HOLD  = 0xF3, // Measure Temperature, No Hold Master Mode
  SI7021_TEMPERATURE_PREV_VAL        = 0xE0, // Read Temperature Value from Previous RH Measurement
  SI7021_RESET                       = 0xFE, // Reset
  SI7021_WRITE_RHT_USER_REG          = 0xE6, // Write RH/T User Register
  SI7021_READ_RHT_USER_REG           = 0xE7, // Read RH/T User Register
  SI7021_WRITE_HEATER_CTRL_REG       = 0x51, // Write Heater Control Register
  SI7021_READ_HEATER_CTRL_REG        = 0x11, // Read Heater Control Register
  SI7021_READ_ID1_L                  = 0xFA, // Read ID 1st Byte
  SI7021_READ_ID1_H                  = 0x0F, // 
  SI7021_READ_ID2_L                  = 0xFC, // Read ID 2nd Byte
  SI7021_READ_ID2_H                  = 0xC9, // 
  SI7021_READ_FIRMWARE_REV_L         = 0x84, // Read Firmware Revision
  SI7021_READ_FIRMWARE_REV_H         = 0xB8  //
}si7021Operations;
uint64_t si7021ReadId(void);
float si7021ReadTemperature(bool isMasterHold);
bool si7021WriteResolution(uint8_t resolution);
#endif
