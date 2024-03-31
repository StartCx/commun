#ifndef __HC_SR04_H
#define __HC_SR04_H


#include "core.h"

typedef struct HC_SR04_Driver_t
{
	CPU_RegisterClass_t  Register;
	Timer_t	 Timer;
	GPIO_TypeDef	*GPIOx_TRIG;		//端口
	uint16_t 		GPIO_Pin_TRIG;	//引脚
	GPIO_TypeDef	*GPIOx_ECHO;		//端口
	uint16_t 		GPIO_Pin_ECHO;	//引脚
}HC_SR04_Driver_t;


extern HC_SR04_Driver_t HC_SR04_Dev;

void HC_SR04_Init(HC_SR04_Driver_t *HC_SR04);
void HC_SR04_Driver(HC_SR04_Driver_t *HC_SR04);



#endif
