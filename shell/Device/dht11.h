#ifndef __DHT11_H
#define __DHT11_H

#include "core.h"



typedef struct DHT11_Driver_t
{
	uint8_t  Start;	//��־λ
	uint8_t  Done;	//��־λ
	CPU_RegisterClass_t  Register;
	GPIO_TypeDef	*GPIOx;		//�˿�
	uint16_t 		GPIO_Pin;	//����
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
	Timer_t	 Timer;
}DHT11_Driver_t;

extern DHT11_Driver_t DHT11_Driver;

void DHT11_GPIO_Config(struct DHT11_Driver_t *Device);
void DHT11_Start(struct DHT11_Driver_t *Device);
void DHT11_Stop(struct DHT11_Driver_t *Device);
void DHT11_Driver_Task(struct DHT11_Driver_t *Device);

#endif
