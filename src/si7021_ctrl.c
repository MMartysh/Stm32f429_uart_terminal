#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "errorHandlers.h"
#include "si7021_ctrl.h"
#include "i2c_ctrl.h"
#include "terminal.h"

static const uint16_t SI7021_ADDRESS = (0x40<<1);           // Si7021 I2C address

uint64_t si7021ReadId(void)
{
    uint8_t id[2][4] = {0};
    uint64_t serialNum = 0;
    bool opStatus = true;
    uint16_t idRegReadCommand[] = {(SI7021_READ_ID1_L | (SI7021_READ_ID1_H << 8)),
                                   (SI7021_READ_ID2_L | (SI7021_READ_ID2_H << 8))};
    static const uint8_t transferSize = sizeof(idRegReadCommand) / sizeof(idRegReadCommand[0]);
    for(uint8_t i = 0; i < (sizeof(idRegReadCommand)/ sizeof(idRegReadCommand[0])); i++)
    {
        opStatus &= i2cWrite(SI7021_ADDRESS, (uint8_t*)&idRegReadCommand[i], sizeof(idRegReadCommand[0]));
        opStatus &= i2cRead(SI7021_ADDRESS,  &id[i][0], sizeof(id[0]));
        for(uint8_t j = 0; j < sizeof(id[0]); j++)
        {
              serialNum <<= 8;
              serialNum |= (uint64_t)id[i][j];
        }
    }
    if (!opStatus) 
    {
        return 0;
    }

    return serialNum;
}


float si7021ReadTemperature(bool isMasterHold)
{
    uint8_t tempRegReadAddr;
    if(!isMasterHold)
    {
        tempRegReadAddr = SI7021_TEMPERATURE_MASTER_NO_HOLD;
    }
    else
    {
        tempRegReadAddr = SI7021_TEMPERATURE_MASTER_HOLD;
    }
    bool opStatus = i2cWrite(SI7021_ADDRESS, &tempRegReadAddr, 1);

    uint8_t temp[2];
    opStatus = opStatus && i2cRead(SI7021_ADDRESS, temp, sizeof(temp));
    if(!opStatus)
    {
        return 0;
    }

    return (175.72 * ((float)((temp[0] << 8) | temp[1])) / 65536.0) - 46.85;
}

float si7021ReadHumidity(bool isMasterHold)
{
    uint8_t tempRegReadAddr;
    if(!isMasterHold)
    {
        tempRegReadAddr = SI7021_HUMIDITY_MASTER_HOLD;
    }
    else
    {
        tempRegReadAddr = SI7021_HUMIDITY_MASTER_NO_HOLD;
    }
    bool opStatus = i2cWrite(SI7021_ADDRESS, &tempRegReadAddr, 1);

    uint8_t humidity[2];
    opStatus = opStatus && i2cRead(SI7021_ADDRESS, humidity, sizeof(humidity));
    if(!opStatus)
    {
        return 0;
    }

    return ((125 * ((float)((humidity[0] << 8) | humidity[1])) / 65536.0) - 6.0);
}

bool si7021Reset(void)
{
    uint8_t cmd = SI7021_RESET;
    return i2cRead(SI7021_ADDRESS, &cmd, 1);
}

uint8_t si7021ReadFirmwareRevision(void)
{
    uint16_t revRegReadCommand = (SI7021_READ_FIRMWARE_REV_L | (SI7021_READ_FIRMWARE_REV_H << 8));
    uint8_t revision;
    bool opStatus = i2cWrite(SI7021_ADDRESS, (uint8_t*)&revRegReadCommand, sizeof(revRegReadCommand));
    opStatus &= i2cRead(SI7021_ADDRESS, &revision, sizeof(revision));
    if(!opStatus)
    {
        return 0;
    }

    return revision;
}

bool si7021WriteResolution(uint8_t resolution)
{
    uint8_t cmd = SI7021_READ_RHT_USER_REG;
    uint8_t regContents;
    bool opStatus = i2cWrite(SI7021_ADDRESS, &cmd, sizeof(cmd));
    opStatus &= i2cRead(SI7021_ADDRESS, &regContents, sizeof(regContents));

    switch (resolution)
    {
        case SI7021_RES_12_RH_14_TEMP:
            cmd = ((regContents & ~(SI7021_RHT_USER_REG_D7 | SI7021_RHT_USER_REG_D0)) << 8);
        break;

        case SI7021_RES_08_RH_12_TEMP:
            cmd = (((regContents & ~SI7021_RHT_USER_REG_D7) | SI7021_RHT_USER_REG_D0) << 8);
        break;

        case SI7021_RES_10_RH_13_TEMP:
            cmd = (((regContents | SI7021_RHT_USER_REG_D7) & ~SI7021_RHT_USER_REG_D0) << 8);
        break;

        case SI7021_RES_11_RH_11_TEMP:
            cmd = ((regContents | SI7021_RHT_USER_REG_D7 | SI7021_RHT_USER_REG_D0) << 8);
        break;
    }
    cmd |= SI7021_WRITE_RHT_USER_REG;
    opStatus &= i2cWrite(SI7021_ADDRESS, (uint8_t*)&cmd, sizeof(cmd));

    return opStatus;
}
/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Return ADC value

 @param[in]     argc nummber of arguments
 @param[in]     argv arguments array

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
bool terminalSi7021GetValue(uint8_t argc, char **argv)
{
    if(argc < 2)
    {
        return false;
    }
    if(strcmp(argv[1], "temperature") == 0)
    {
        printf("Temperature: %f C\n", si7021ReadTemperature(false));
    }
    else if(strcmp(argv[1], "humidity") == 0)
    {
        printf("Humidity: %f \n", si7021ReadHumidity(false));
    }
    else if(strcmp(argv[1], "id") == 0)
    {
         printf("ID: 0x%llx \n", si7021ReadId());
    }
    else if(strcmp(argv[1], "firmware") == 0)
    {
         printf("Firmware rev: 0x%x \n", si7021ReadFirmwareRevision());
    }
    return true;
}

/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Add terminal command handle to list
*/
/* ----------------------------------------------------------------------------
 */
__attribute__((constructor))
void terminalTempInit(void)
{
  static commandStruct tempCommand = 
  { 
          .name = "si7021",
          .description = "Perform read or write opeartions with temp sensor. \ntemp",
          .callback = terminalSi7021GetValue,
          .next = NULL 
  };
  terminalAddCommand(&tempCommand);
}