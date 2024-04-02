#ifndef __REGISTER_H
#define __REGISTER_H


#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_i2c.h"
#include "usart.h"
#include "adc.h"


#define GPIO_SET_HIGH(GPIOx, GPIO_Pin) GPIO_SetBits(GPIOx, GPIO_Pin)
#define GPIO_SET_LOW(GPIOx, GPIO_Pin)  GPIO_ResetBits(GPIOx, GPIO_Pin)
#define GPIO_GET_STATE(GPIOx, GPIO_Pin)  GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)


#define NULL 0
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;
typedef float 				   float32_t;
typedef double 				   float64_t;



enum
{
	CORE_RUNNING = 0,
	CORE_DONE    = 1,
	CORE_READY   = 2,
	CORE_BUSY    = 3,
	CORE_SUCCESS = 4,
	CORE_ERROR	 = 5,
	CORE_LOCK	 = 6,//����
	CORE_UNLOCK	 = 7,//����
	CORE_OK	 	 = 8,
	CORE_TIMEOUT = 9,
	CORE_ABORT 	 = 10,
	CORE_EXIT 	 = 11,
	CORE_SET 	 = 12,
	CORE_SET_DONE= 13,
};

extern uint32_t get_init_fp(void);

#define stack_size	1024
#define stack_top_addr  get_init_fp()
#define stack_top_sp    __get_MSP()
#define stack_bottom_fp stack_top_addr-stack_size


typedef struct
{
	uint32_t min_usage_stack;
	uint32_t max_usage_stack;
	uint32_t cur_usage_stack;
}StackInfo_TypeDef;


typedef struct
{
	int32_t    R0_Result;//���н��
	uint32_t   R1_Index; //����
	uint8_t    R2_cin;	//��׼����
	uint8_t    R3_cout;	//��׼���
	uint16_t   R4_Status;//״̬����
	uint16_t   R5_Object;//����Ŀ��
	uint32_t   R6_Count;//������
	uint16_t   R7_ACC;	//�ۼ���
	uint16_t   R8_Flag;	//��־λ
	uint16_t   R9_Error;//�������
	uint32_t   R10_Mask;//λ����
	uint16_t   R11_ch;	//char����
	uint16_t   R12_Lock;//��Դ��
	uint16_t   R13_SP;	//ջ��
	uint16_t   R14_LR;	//���ص�ַ
	uint16_t   R15_PC;	//ָ���ַ
	uint16_t   Bus;		//����
	uint16_t   Device;	//�豸
	uint32_t   *Pointer;
	uint32_t   TimerTick;
	uint32_t   Address;
	uint8_t    ret;		//api����ֵ
	uint32_t   retry;	//���Դ���
}CPU_RegisterClass_t;

typedef struct Timer_t
{
	uint32_t TickCount;
	uint32_t TickPrev;
	uint32_t TickPeroid;
	uint32_t TickTime;
}Timer_t;


extern uint32_t Cur_Usage_Cpu;
extern uint32_t Cur_Usage_Max_Cpu;


extern void Start_CPU(void);
extern void Stop_CPU(uint32_t Time);
extern void Timer_IncTick(void);
extern uint8_t Scheduler(struct Timer_t *p);
extern void Restart_Scheduler_Timer(struct Timer_t *p);
extern void GPIO_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


#endif
