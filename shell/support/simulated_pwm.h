#ifndef __SIMULATED_PWM_H
#define __SIMULATED_PWM_H


#include "core.h"

typedef struct Simulated_PWM_t
{
	uint16_t Pwm_Preiod;
	uint16_t PwmPreiodCnt;
	uint16_t PwmHighPulse;
	
	GPIO_TypeDef	*GPIOx;		//¶Ë¿Ú
	uint16_t 		GPIO_Pin;	//Òý½Å
	
}Simulated_PWM_t;

extern Simulated_PWM_t Simulated_PWM;

extern void PWM_GPIO_Init(Simulated_PWM_t *Simulated_PWM);
extern void PWM_Out_Put(Simulated_PWM_t *Simulated_PWM);
extern void PWM_Set_HighPulse(Simulated_PWM_t *Simulated_PWM, uint16_t PwmHighPulse);


#endif 
