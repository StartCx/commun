#ifndef __OUTPUT_LED_H
#define __OUTPUT_LED_H

#include "stm32f10x.h"
#include "core.h"

enum
{
	GPIO_LOW = 0,
	GPIO_HIGH,
	GPIO_HPD,
	GPIO_TOGGLE,
	GPIO_MODE_SUM,
};


typedef struct GPIO_Device_Class_t
{
	//����
	GPIO_TypeDef	*GPIOx;		//�˿�
	uint16_t 		GPIO_Pin;	//����
	uint16_t 		*duty;		//ռ�ձ�
	uint16_t 		Timer;		//��ǰʱ��
	uint16_t 		LightDarkPeriod;//����
}GPIO_Device_Class_t;


typedef struct GPIO_Output_Device_t
{
	//����
	CPU_RegisterClass_t Register; //���̿��ƿ�
	GPIO_Device_Class_t Attribute;//�豸���Կ�
	Timer_t				Timer;
	//����
	void (*Init)(struct GPIO_Output_Device_t *Dev);
	void (*Set)(struct GPIO_Output_Device_t *Dev, uint8_t mode);
	void (*Driver)(struct GPIO_Output_Device_t *Dev);
	void (*Quit)(struct GPIO_Output_Device_t *Dev);

}GPIO_Output_Device_t;

extern GPIO_Output_Device_t Led_Device;
extern void GPIO_Out_Init(GPIO_Output_Device_t *Dev);
extern void GPIO_Out_Driver(GPIO_Output_Device_t *Dev);
extern void GPIO_Out_Set(GPIO_Output_Device_t *Dev, uint8_t mode);
extern void GPIO_Out_Quit(GPIO_Output_Device_t *Dev);

#endif
