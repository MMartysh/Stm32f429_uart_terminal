// Standard includes
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
// HAL includes
#include "stm32f4xx_hal.h"
// Project includes
#include "timer_ctrl.h"
#include "i2c_ctrl.h"
#include "at24xx.h"
#include "terminal.h"

/*! 
 @brief Temporary buffer for read/write 
 
 @note The size of array is define as the largest page possible + 2 bytes max for address 
*/
uint8_t at24_buffer[(uint32_t)Byte_256 + 2];

/*! EEPROM instance */
const at24_EEPROM eeprom = {
    .i2c = 1,
    .address = 0x50,
    .capacity = KiBit_256,
    .page_size = Byte_64
};
static struct 
{
    uint32_t address;
    uint32_t size;
}testData;
static timerHandler eepromTimer;
static bool terminalEeprom (uint8_t argc, char **argv);


/*----------------------------------------------------------------------------*/
/*! 
 @brief Write data to EEPROM
 
 @param [in] eepromInstance Pointer to at24_EEPROM instance
 @param [in] addr Starting write address
 @param [in] tx_buf pointer to buffer with datat to send
 @param [in] len Number of bytes to write
 @return Returns the number of transmitted bytes
*/
/*----------------------------------------------------------------------------*/
uint32_t at24_Write(const at24_EEPROM *eepromInstance, uint32_t addr, const uint8_t *tx_buf, uint32_t len)
{  
    uint32_t tx_count = 0;

    if ((len == 0) || ((addr + len) > eepromInstance->capacity))
    {
        return 0;
    }

    while (tx_count < len)
    {

        uint32_t page_num      = (addr / eepromInstance->page_size);
        uint32_t page_addr_min = page_num * eepromInstance->page_size;
        uint32_t page_addr_max = (page_addr_min + eepromInstance->page_size) - 1;

        uint32_t len_diff      = len - tx_count;
        uint32_t addr_start    = addr;
        uint32_t addr_stop     = ((addr + len_diff) - 1) < page_addr_max ? ((addr + len_diff) - 1) : page_addr_max;
        uint32_t data_len      = (addr_stop - addr_start) + 1;
        
        //Copy write address and data to a buffer
        uint16_t bytesToWrite = eepromInstance->capacity > KiBit_16 ? ((uint16_t)data_len + 2) : ((uint16_t)data_len + 1);
       

        //Fill the buffer. 1 or 2 first bytes depends on chip's capacity is the address
        if(eepromInstance->capacity >= KiBit_32)
        {
            //Copy write address
            at24_buffer[0] = (uint8_t)(addr >> 8U);
            at24_buffer[1] = (uint8_t)addr;
            //Copy data to write
            memcpy(&at24_buffer[2], &tx_buf[tx_count], data_len);
        }
        else
        {
            //Copy write address
            at24_buffer[0] = (uint8_t)addr;
            //Copy data to write
            memcpy(&at24_buffer[1], &tx_buf[tx_count], data_len);
        }

        //For some devices a part of the address is set in the device address
        uint32_t device_address = eepromInstance->address;
        if((eepromInstance->capacity >= KiBit_4) && (eepromInstance->capacity <= KiBit_16))
        {
            device_address |= (addr & 0x700UL) >> 8U;
        }
        if(eepromInstance->capacity > KiBit_512)
        {
            device_address |=  (addr & 0x70000UL) >> 16U;
        }
        
        // Write page data
        if(!i2cWrite((uint16_t)(device_address << 1), at24_buffer, bytesToWrite))
        {            
            break;
        }
        //Page write needs some time for internal write cycle inside the EEPROM chip
        //The divice will response NACK during this time
        //Perform a dummy read until get ACK
        uint32_t tmp = 0;
        uint8_t dummy;
        uint32_t count = 0;
        do{
            HAL_Delay(5);
            tmp = at24_Read(eepromInstance, 0, &dummy, 1);
            count++;
        }while ((tmp == 0) && (count < 200));
        
        // Didn't get response until timeout
        if(count >= 200)
        {
            break;
        }
        
        tx_count += data_len;
        addr += data_len;

    }
  
    return tx_count;

}

/*----------------------------------------------------------------------------*/
/*! 
 @brief Reads data to EEPROM
 
 @param [in] eeprom Pointer to at24_EEPROM instance
 @param [in] addr Starting read address
 @param [in] rx_buf pointer to store read data
 @param [in] len Number of bytes to read
 @return Returns the number of read bytes
*/
/*----------------------------------------------------------------------------*/
uint32_t at24_Read(const at24_EEPROM *eepromInstance, uint32_t addr, uint8_t *rx_buf, uint32_t len)
{

    if ((len == 0) || ((addr + len) > eepromInstance->capacity))
    {
        return 0;
    }
    
    uint32_t addr_start = addr;
    //Devices with capacity more than 32k use 2-bytes addresses. Bytes need to be switched in this case
    if(eepromInstance->capacity >= KiBit_32)
    {
        addr_start = ((addr_start & 0x00FF) << 8U) | ((addr_start & 0xFF00) >> 8U); 
    }

    //For some devices a part of the address is set in the device address
    uint32_t device_address = eepromInstance->address;
    if((eepromInstance->capacity >= KiBit_4) && (eepromInstance->capacity <= KiBit_16))
    {
        device_address |= (addr & 0x700UL) >> 8U;
    }
    if(eepromInstance->capacity > KiBit_512)
    {
        device_address |=  (addr & 0x70000UL) >> 16U;
    }
    
    // Read data
    uint16_t bytesToWrite = eepromInstance->capacity > KiBit_32 ? 2 : 1;
    if(!i2cWrite((uint16_t)(device_address << 1), (uint8_t *)&addr_start, bytesToWrite))
    {
        return 0;
    }

    return len;

}


/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Add terminal command handle to list
*/
/* ----------------------------------------------------------------------------
 */
__attribute__((constructor))
void eepromInit(void)
{
    static commandStruct terminalEepromItem = 
    { 
        .name = "eeprom",
        .description =  "Writes/Reads data of given size into/from EEPROM\n\n  eeprom "
                "[arg1] - read or write modifier, [arg4]...[arg n] - arguments to write(ignored in case of reading)\n"
                "arg2 arg3...arg n \n\n\t arg2 - starting address ,arg3 - size",
                
        .callback = terminalEeprom,
        .next = NULL 
    };
    terminalAddCommand(&terminalEepromItem);
}

/*----------------------------------------------------------------------------*/
/*!
 @brief         Timer handler of EEPROm operations
*/
/*----------------------------------------------------------------------------*/
static void eepromOperation(void)
{
    uint8_t buff[16];
    uint32_t amount = sizeof(buff) / sizeof(buff[0]);
    
    if(testData.size <= amount)
    {
        amount = testData.size;
    }
    // Read portion
    if(at24_Read(&eeprom, testData.address, buff, amount) != amount)
    {
        printf("Read error at address %d\n", testData.address);
        timerRemove(&eepromTimer);
        return;
    }
    printf("0x%08X: ", testData.address);
    testData.size -= amount;
    testData.address += amount;
    
    for(uint32_t i = 0; i < amount; i++)
    {
        printf("0x%02X ", buff[i]);
    } 
    printf("\n");
    
    if(testData.size == 0)
    {
        timerRemove(&eepromTimer);
    }
    
}

/*----------------------------------------------------------------------------*/
/*! 
 @brief Reads data to EEPROM
 
 @param [in] size size to read

 @return true if success, false otherwise
*/
/*----------------------------------------------------------------------------*/
static bool eepromRead(uint32_t size)
{
    testData.size = size;
    //checking whether size is not higher than max EEPROM capacity
    if((testData.address + testData.size) > eeprom.capacity)
    {
        printf("Address out of available memory!\n");
        return false;
    }
    eepromTimer.callback = eepromOperation;
    eepromTimer.periodMs = 10;
    timerAdd(&eepromTimer);
    return true;
}
static uint32_t eepromWrite(uint8_t argc, char **argv)
{
    uint8_t buff[16];
    uint32_t amount = sizeof(buff) / sizeof(buff[0]);
    testData.size = argc - 3;
    uint32_t bytesWritten = 0;
    // Check whether size is not higher than max EEPROM capacity
    if((testData.address + testData.size) > eeprom.capacity)
    {
        return false;
    }
    while(testData.size > 0)
    {
        if(testData.size <= amount)
        {
            amount = testData.size;
        }
        // Get data to write
        for(uint32_t i = 0; i < amount; i++)
        {
            uint32_t tmp;
            if(!terminalGetInt(argv[(i + 3) + bytesWritten], &tmp))
            {
                printf("Cannot convert %s to number\n", argv[(i + 3) + bytesWritten]);
                return false;
            }
            buff[i] = (uint8_t)tmp;
        }
        // Write data
        if(at24_Write(&eeprom, testData.address, buff, amount) != amount)
        {
            return false;
        }
        testData.size -= amount;
        testData.address += amount;
        bytesWritten += amount;
    } 
    
    printf("Written successfully\n");
    return true;
}
/* ----------------------------------------------------------------------------
 */
/*!
 @brief         Reads/Writes EEPROM data

 @param[in]     argc number of command arguments
 @param[in]     argv command arguments

 @return        Status of operation
*/
/* ----------------------------------------------------------------------------
 */
static bool terminalEeprom (uint8_t argc, char **argv)
{
    //if arguments number is not sufficient for this operation
    if(argc < 3)
    {
        return false;
    }
    // Second argument is start address
    if (!terminalGetInt(argv[2], &testData.address))
    {
        printf("Cannot convert %s to number\n", argv[2]);
        return false;
    }
    if(strcmp(argv[1], "read") == 0)
    {
        uint32_t dataSize;
        if(!terminalGetInt(argv[3], &dataSize))
        {
            printf("Cannot convert %s to number\n", argv[3]);
            return false;
        }
        eepromRead(dataSize);
    }
    else if(strcmp(argv[1], "write") == 0)
    {
        eepromWrite(argc, argv);
    }
    else
    {
        return false;
    }
    return true;
}





