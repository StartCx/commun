#ifndef __SERVO_OUTPUT_H
#define __SERVO_OUTPUT_H


#include "core.h"


typedef struct
{
	GPIO_TypeDef	*GPIOx;		//�˿�
	uint16_t 		GPIO_Pin;	//����
	uint16_t 		target_duty;//Ŀ��ռ�ձ�
	uint16_t 		cur_duty;	//��ǰռ�ձ�
	uint16_t 		period;		//����
	uint16_t 		count;		//���ڼ�����
	Timer_t			Timer;
}Servo_Output_t;

extern Servo_Output_t Servo_Output;


void Servo_GPIO_Init(Servo_Output_t * Servo_Output);
void servo_angle_set(Servo_Output_t *Servo_Output, uint16_t duty);
void servo_output_angle_task(Servo_Output_t *Servo_Output);
void servo_output_speed_task(Servo_Output_t *Servo_Output);


#endif
