#ifndef TIMER_CTRL_H
#define TIMER_CTRL_H

typedef struct timerHandler
{
    uint8_t id;                  //timer id, used for deletion
    uint32_t periodMs;           //timer period is milliseconds
    uint32_t counter;            //counter for checking whether timer callback needs to be called
    void (*callback)(void);      //function to call after each period
    struct timerHandler *next;   //next timer handler        
}timerHandler;

void timerInit(void);
void timerPerformCheck(void);
void timerAdd(timerHandler *timerToAdd);
void timerRemove(timerHandler *timerToRemove);
#endif
