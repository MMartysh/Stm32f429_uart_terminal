#ifndef PWM_CTRL_H
#define PWM_CTRL_H

void 			HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void 			pwmInit(void);
void 			pwmStart(uint32_t dutyCycle);
void 			pwmStop(void);

#endif
