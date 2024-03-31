#ifndef __SERVO_OUTPUT_H
#define __SERVO_OUTPUT_H


#include "core.h"


typedef struct
{
	GPIO_TypeDef	*GPIOx;		//端口
	uint16_t 		GPIO_Pin;	//引脚
	uint16_t 		target_duty;//目标占空比
	uint16_t 		cur_duty;	//当前占空比
	uint16_t 		period;		//周期
	uint16_t 		count;		//周期计数器
	Timer_t			Timer;
}Servo_Output_t;

extern Servo_Output_t Servo_Output;


void Servo_GPIO_Init(Servo_Output_t * Servo_Output);
void servo_angle_set(Servo_Output_t *Servo_Output, uint16_t duty);
void servo_output_angle_task(Servo_Output_t *Servo_Output);
void servo_output_speed_task(Servo_Output_t *Servo_Output);


#endif
