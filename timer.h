#ifndef TIMER_H
#define TIMER_H


void SysTick_Handler(void);
void TIM3_Init(void);
HAL_StatusTypeDef TIMER_Start(void);
float TIMER_GetTime(void);

#endif
