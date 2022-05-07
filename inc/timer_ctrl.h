#ifndef TIMER_CTRL_H
#define TIMER_CTRL_H

void SysTick_Handler(void);
void timerInit(void);
HAL_StatusTypeDef timerStart(void);

#endif
