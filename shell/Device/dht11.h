#ifndef __DHT11_H
#define __DHT11_H

#include "core.h"



typedef struct DHT11_Driver_t
{
	uint8_t  Start;	//标志位
	uint8_t  Done;	//标志位
	CPU_RegisterClass_t  Register;
	GPIO_TypeDef	*GPIOx;		//端口
	uint16_t 		GPIO_Pin;	//引脚
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
	Timer_t	 Timer;
}DHT11_Driver_t;

extern DHT11_Driver_t DHT11_Driver;

void DHT11_GPIO_Config(struct DHT11_Driver_t *Device);
void DHT11_Start(struct DHT11_Driver_t *Device);
void DHT11_Stop(struct DHT11_Driver_t *Device);
void DHT11_Driver_Task(struct DHT11_Driver_t *Device);

#endif
