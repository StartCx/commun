#ifndef __HC_SR04_H
#define __HC_SR04_H


#include "core.h"

typedef struct HC_SR04_Driver_t
{
	CPU_RegisterClass_t  Register;
	Timer_t	 Timer;
	GPIO_TypeDef	*GPIOx_TRIG;		//�˿�
	uint16_t 		GPIO_Pin_TRIG;	//����
	GPIO_TypeDef	*GPIOx_ECHO;		//�˿�
	uint16_t 		GPIO_Pin_ECHO;	//����
}HC_SR04_Driver_t;


extern HC_SR04_Driver_t HC_SR04_Dev;

void HC_SR04_Init(HC_SR04_Driver_t *HC_SR04);
void HC_SR04_Driver(HC_SR04_Driver_t *HC_SR04);



#endif
