#ifndef PWM_H
#define PWM_H

void 			HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void 			TIM1_Init(void);
void 			PWM_Start(uint32_t p_ui_DutyCycle);
void 			PWM_Stop(void);

#endif
