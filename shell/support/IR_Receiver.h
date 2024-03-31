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
	//属性
	GPIO_TypeDef	*GPIOx;		//端口
	uint16_t 		GPIO_Pin;	//引脚
	CPU_RegisterClass_t Register;
	Timer_t				Timer;
	Queue256_t 			Queue;
	uint8_t 		Leader_Code[2];	//引脚
	uint8_t 		Code_Low_time[32];	//引脚
	uint8_t 		Code_High_time[32];	//引脚
}IR_receiver_t;

extern IR_receiver_t IR_Register;

void IR_RECV_Init(IR_receiver_t *IR_Register);
void IR_RECV_Driver(IR_receiver_t *IR_Register);



#endif
