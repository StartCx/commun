#ifndef DS18B20_H
#define DS18B20_H

#include "core.h"


typedef struct DS18B20_Driver_t
{
	uint8_t  Start;	//标志位
	uint8_t  Done;	//标志位
	CPU_RegisterClass_t  Register;
	GPIO_TypeDef	*GPIOx;		//端口
	uint16_t 		GPIO_Pin;	//引脚
	Timer_t	 Timer;
	uint8_t TH;
	uint8_t TL;
	int16_t Temp;
}DS18B20_Driver_t;

extern DS18B20_Driver_t DS18B20_Dev;
void DS18B20_Start(DS18B20_Driver_t *DS18B20_Device);
void DS18B20_Stop(DS18B20_Driver_t *DS18B20_Device);
void DS18B20_Init(DS18B20_Driver_t *DS18B20_Device);
void DS18B20_Get_Temp(DS18B20_Driver_t *DS18B20_Device);

#endif

