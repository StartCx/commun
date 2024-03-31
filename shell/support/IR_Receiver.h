#ifndef __IR_RECEIVER_H
#define __IR_RECEIVER_H

#include "core.h"
#include "queue.h"

enum
{
	INVALID_CODE,
	POWER_CODE,
	ADD_CODE,
	SUB_CODE,
	REPEAT_CODE,
};

typedef struct IR_receiver_t
{
	//����
	GPIO_TypeDef	*GPIOx;		//�˿�
	uint16_t 		GPIO_Pin;	//����
	CPU_RegisterClass_t Register;
	Timer_t				Timer;
	Queue256_t 			Queue;
	uint8_t 		Leader_Code[2];	//����
	uint8_t 		Code_Low_time[32];	//����
	uint8_t 		Code_High_time[32];	//����
}IR_receiver_t;

extern IR_receiver_t IR_Register;

void IR_RECV_Init(IR_receiver_t *IR_Register);
void IR_RECV_Driver(IR_receiver_t *IR_Register);



#endif
