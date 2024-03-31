#include "simulated_pwm.h"

Simulated_PWM_t Simulated_PWM = {
	.Pwm_Preiod = 100,
	.PwmHighPulse = 50,
	.GPIOx      = GPIOC,
	.GPIO_Pin   = GPIO_Pin_14,
};

void PWM_GPIO_Init(Simulated_PWM_t *Simulated_PWM)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = Simulated_PWM->GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Simulated_PWM->GPIOx, &GPIO_InitStructure);
}


void PWM_Out_Put(Simulated_PWM_t *Simulated_PWM)
{
	if( Simulated_PWM->PwmHighPulse == 0){
		GPIO_SET_LOW(Simulated_PWM->GPIOx, Simulated_PWM->GPIO_Pin);
		return;
	}else if( Simulated_PWM->PwmPreiodCnt <= Simulated_PWM->PwmHighPulse){     
        GPIO_SET_HIGH(Simulated_PWM->GPIOx, Simulated_PWM->GPIO_Pin);
    }else{
		GPIO_SET_LOW(Simulated_PWM->GPIOx, Simulated_PWM->GPIO_Pin);
    }
    if( (++Simulated_PWM->PwmPreiodCnt) > Simulated_PWM->Pwm_Preiod){
        Simulated_PWM->PwmPreiodCnt = 0; 
    }
}

void PWM_Set_HighPulse(Simulated_PWM_t *Simulated_PWM, uint16_t PwmHighPulse)
{
	if( PwmHighPulse > Simulated_PWM->Pwm_Preiod){
		PwmHighPulse = Simulated_PWM->Pwm_Preiod;
	}
	Simulated_PWM->PwmHighPulse = PwmHighPulse;
}

