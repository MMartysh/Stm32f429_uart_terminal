#ifndef PWM_H
#define PWM_H

void MX_TIM1_Init(uint32_t pwm_pulse);
void startPWM(void);
void stopPWM(void);
#endif