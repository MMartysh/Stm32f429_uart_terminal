#ifndef I2C_CTRL_H
#define I2C_CTRL_H

void i2cInit(void);
bool i2cRead(uint16_t address, uint8_t *data, uint16_t size);
bool i2cWrite(uint16_t address, uint8_t *data, uint16_t size);
I2C_HandleTypeDef *i2cGetHandle(void);
#endif
