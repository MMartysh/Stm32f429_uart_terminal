#ifndef LCD1602A_CTRL_H
#define LCD1602A_CTRL_H

typedef union lcd1602CommandBits
{
    uint16_t value;
    struct
    {                               
        uint16_t moveDir               : 1;
        uint16_t displayShift          : 1;
        uint16_t putOneHere            : 1;
        uint16_t reserved              : 13; 
    }entryMode;                              
    struct             
    {                               
        uint16_t displayOnOff        : 1;
        uint16_t cursorOnOff         : 1;
        uint16_t cursorBlinking      : 1;
        uint16_t putOneHere          : 1;
        uint16_t reserved            : 12; 
    }displaySettings;
    struct 
    {
        uint16_t reserved0           : 2;
        uint16_t cursorMoveDispShift : 1;
        uint16_t leftRightShift      : 1;
        uint16_t putOneHere          : 1;
        uint16_t reserved1           : 11; 
    }cursorDisplayShift;
    struct 
    {
        uint16_t reserved1           : 2;
        uint16_t font                : 1;
        uint16_t displayLines        : 1;
        uint16_t dataLength          : 1;
        uint16_t putOneHere          : 1;
        uint16_t reserved2           : 10; 
    }functionSet;
    struct 
    {
        uint16_t addr                : 6;
        uint16_t putOneHere          : 1;
        uint16_t reserved            : 9; 
    }setCgramAddr;
    struct 
    {
        uint16_t addr                : 7;
        uint16_t putOneHere          : 1;
        uint16_t reserved            : 8; 
    }setDdramAddr;
    struct 
    {
        uint16_t addrCounter         : 7;
        uint16_t busyFlag            : 1;
        uint16_t putOneHere          : 1;
        uint16_t reserved            : 7; 
    }readBusyFlagAddr;
    struct 
    {
        uint16_t symbol              : 8;
        uint16_t reserved1           : 1; 
        uint16_t putOneHere          : 1;
        uint16_t reserved2           : 1;
    }displaySymbol;
}lcd1602CommandBits;

#endif
