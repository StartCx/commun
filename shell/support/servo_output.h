#ifndef __SERVO_OUTPUT_H
#define __SERVO_OUTPUT_H


#include "core.h"


typedef struct
{
	GPIO_TypeDef	*GPIOx;		//端口
	uint16_t 		GPIO_Pin;	//引脚
	uint16_t 		target_duty;//目标占空比
	uint16_t 		cur_duty;	//当前占空比
	uint16_t 		shadow_reg;	//影子寄存器
	uint16_t 		period;		//周期
	uint16_t 		count;		//周期计数器
	Timer_t			Timer;
}Sim_Servo_Output_t;


typedef struct
{
	GPIO_TypeDef	*GPIOx;		//端口
	uint16_t 		GPIO_Pin;	//引脚
	uint16_t 		target_duty;//目标占空比
	uint16_t 		cur_duty;	//当前占空比
	Timer_t			Timer;
	void (*TIM_SetCompare)(TIM_TypeDef* TIMx, uint16_t Compare4);
}PWM_Servo_Output_t;


extern Sim_Servo_Output_t Servo_Output0;
extern PWM_Servo_Output_t Servo_Output1;

void SIM_Servo_GPIO_Init(Sim_Servo_Output_t * Servo_Output);
void sim_servo_angle_set(Sim_Servo_Output_t *Servo_Output, uint16_t duty);
void sim_servo_output_angle_task(Sim_Servo_Output_t *Servo_Output);
void sim_servo_output_speed_task(Sim_Servo_Output_t *Servo_Output);
void pwm_servo_output_speed_task(PWM_Servo_Output_t *Servo_Output);
void pwm_servo_angle_set(PWM_Servo_Output_t *Servo_Output, uint16_t duty);

#endif
