#include "servo_output.h"


Servo_Output_t Servo_Output = {
	.period = 2000, //10us一次，20000us = 20ms，周期。
	.target_duty = 50,
	.cur_duty    = 50,
	.GPIOx  	= GPIOC,
	.GPIO_Pin 	= GPIO_Pin_15,
	.Timer = {
		.TickPeroid = 500,//10*1000等于10ms执行一次。
	},
};

void Servo_GPIO_Init(Servo_Output_t * Servo_Output)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = Servo_Output->GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Servo_Output->GPIOx, &GPIO_InitStructure);
}



void servo_angle_set(Servo_Output_t *Servo_Output, uint16_t duty)
{
	if( duty < 250 && duty != NULL){
		Servo_Output->target_duty = duty;
	}
}



void servo_output_angle_task(Servo_Output_t *Servo_Output)
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


void servo_output_speed_task(Servo_Output_t *Servo_Output)
{
	if( Servo_Output->cur_duty < Servo_Output->target_duty){
		Servo_Output->cur_duty++;
	}else if( Servo_Output->cur_duty > Servo_Output->target_duty){
		Servo_Output->cur_duty--;
	}
}
