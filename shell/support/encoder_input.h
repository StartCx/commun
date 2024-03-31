#ifndef __ENCODER_INPUT_H
#define __ENCODER_INPUT_H


#include "stm32f10x.h"
#include "core.h"


typedef struct
{
	GPIO_TypeDef	*A_GPIOx;	//端口
	uint16_t 		A_GPIO_Pin;	//引脚
	
	GPIO_TypeDef	*B_GPIOx;	//端口
	uint16_t 		B_GPIO_Pin;	//引脚
	
	CPU_RegisterClass_t Register; //流程控制块
	int16_t 		cur_encode;
	Timer_t			Timer;
}Encoder_Input_Device_t;

extern Encoder_Input_Device_t Encoder;
extern void ENCODE_GPIO_Init(Encoder_Input_Device_t *Encoder);
extern void encode_count_read(Encoder_Input_Device_t *Encoder);


#endif
