#include "servo_output.h"


Sim_Servo_Output_t Servo_Output0 = {
	.period = 2000, //10us一次，20000us = 20ms，周期。
	.target_duty = 50,
	.cur_duty    = 50,
	.GPIOx  	= GPIOA,
	.GPIO_Pin 	= GPIO_Pin_8,
	.Timer = {
		.TickPeroid = 500,//10*1000等于10ms执行一次。
	},
};

PWM_Servo_Output_t Servo_Output1 = {
	.target_duty = 50,
	.cur_duty    = 50,
	.GPIOx  	= GPIOB,
	.GPIO_Pin 	= GPIO_Pin_0,
	.Timer = {
		.TickPeroid = 500,//10*1000等于10ms执行一次。
	},
	.TIM_SetCompare = TIM_SetCompare3,
};


PWM_Servo_Output_t Servo_Output2 = {
	.target_duty = 50,
	.cur_duty    = 50,
	.GPIOx  	= GPIOB,
	.GPIO_Pin 	= GPIO_Pin_1,
	.Timer = {
		.TickPeroid = 500,//10*1000等于10ms执行一次。
	},
	.TIM_SetCompare = TIM_SetCompare4,
};

void SIM_Servo_GPIO_Init(Sim_Servo_Output_t * Servo_Output)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = Servo_Output->GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Servo_Output->GPIOx, &GPIO_InitStructure);
}



void sim_servo_angle_set(Sim_Servo_Output_t *Servo_Output, uint16_t duty)
{
	Servo_Output->target_duty = 50+(200*duty)/180;
}



void sim_servo_output_angle_task(Sim_Servo_Output_t *Servo_Output)
{
	if( Servo_Output->count == 0){
		Servo_Output->count = Servo_Output->period;//10us*2000 = 20ms
	}else{
		Servo_Output->count--;
	}
	if( Servo_Output->count < Servo_Output->cur_duty){
		GPIO_SET_HIGH(Servo_Output->GPIOx, Servo_Output->GPIO_Pin);
	}else{
		GPIO_SET_LOW(Servo_Output->GPIOx, Servo_Output->GPIO_Pin);
	}
}


void sim_servo_output_speed_task(Sim_Servo_Output_t *Servo_Output)
{
	if( Servo_Output->cur_duty < Servo_Output->target_duty){
		Servo_Output->cur_duty++;
	}else if( Servo_Output->cur_duty > Servo_Output->target_duty){
		Servo_Output->cur_duty--;
	}
}


void pwm_servo_output_speed_task(PWM_Servo_Output_t *Servo_Output)
{
	if( Servo_Output->cur_duty < Servo_Output->target_duty){
		Servo_Output->cur_duty++;
		goto LABEL;
	}else if( Servo_Output->cur_duty > Servo_Output->target_duty){
		Servo_Output->cur_duty--;
		goto LABEL;
	}else{
		return;
	}
LABEL:
	Servo_Output->TIM_SetCompare(TIM3,Servo_Output->cur_duty);	   //设置舵机占空比，控制舵机转动
}

void pwm_servo_angle_set(PWM_Servo_Output_t *Servo_Output, uint16_t duty)
{
	Servo_Output->target_duty = 50+(200*duty)/180;
}
