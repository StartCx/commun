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
	CORE_LOCK	 = 6,//上锁
	CORE_UNLOCK	 = 7,//解锁
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
	int32_t    R0_Result;//运行结果
	uint32_t   R1_Index; //索引
	uint8_t    R2_cin;	//标准输入
	uint8_t    R3_cout;	//标准输出
	uint16_t   R4_Status;//状态变量
	uint16_t   R5_Object;//对象目标
	uint32_t   R6_Count;//计数器
	uint16_t   R7_ACC;	//累加器
	uint16_t   R8_Flag;	//标志位
	uint16_t   R9_Error;//错误代码
	uint32_t   R10_Mask;//位掩码
	uint16_t   R11_ch;	//char变量
	uint16_t   R12_Lock;//资源锁
	uint16_t   R13_SP;	//栈顶
	uint16_t   R14_LR;	//返回地址
	uint16_t   R15_PC;	//指令地址
	uint16_t   Bus;		//总线
	uint16_t   Device;	//设备
	uint32_t   *Pointer;
	uint32_t   TimerTick;
	uint32_t   Address;
	uint8_t    ret;		//api返回值
	uint32_t   retry;	//重试次数
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
