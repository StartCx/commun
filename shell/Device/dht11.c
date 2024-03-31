#include "dht11.h"


DHT11_Driver_t DHT11_Driver = {
	.GPIOx 		= GPIOB,
	.GPIO_Pin 	= GPIO_Pin_0,
	.Register.TimerTick = 500000,//5s
	.Register.R6_Count = 0,
};


void DHT11_GPIO_Config (struct DHT11_Driver_t *Device)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
  	GPIO_InitStructure.GPIO_Pin = Device->GPIO_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(Device->GPIOx,&GPIO_InitStructure);
}

void DHT11_Start(struct DHT11_Driver_t *Device)
{		
	Device->Start = 1;
	Device->Done  = 0;
}

void DHT11_Stop(struct DHT11_Driver_t *Device)
{		
	Device->Start = 0;
	Device->Done  = 0;
}


void DHT11_Driver_Task(struct DHT11_Driver_t *Device)
{	
	enum
	{
		DHT11_PROC_INIT,
		DHT11_PROC_GPOUP0,
		DHT11_PROC_GPOUP1,
		DHT11_PROC_GPOUP2,
		DHT11_PROC_GPOUP3,
		DHT11_PROC_GPOUP4,
		DHT11_PROC_GPOUP5,
		DHT11_PROC_GPOUP6,
		DHT11_PROC_GPOUP7,
		DHT11_PROC_GPOUP8,
		DHT11_PROC_GPOUP10,
		DHT11_PROC_GPOUP12,
		DHT11_PROC_GPOUP14,
		DHT11_PROC_GPOUP15,
		DHT11_PROC_GPOUP16,
		
		DHT11_PROC_READ_BYTE_GROUP0,
		DHT11_PROC_READ_BYTE_GROUP2,
		DHT11_PROC_READ_BYTE_GROUP3,
		DHT11_PROC_READ_BYTE_GROUP4,
		DHT11_PROC_READ_BYTE_GROUP5,
		
		DHT11_PROC_READ_ERROR,
		DHT11_PROC_DELAY0,
		DHT11_PROC_DELAY1,
		DHT11_PROC_ENDP,
		DHT11_PROC_TASK_SUM,
	};
	static const void *function[DHT11_PROC_TASK_SUM] = {
		[DHT11_PROC_INIT] 			= &&DHT11_PROC_INIT,
		[DHT11_PROC_GPOUP0] 		= &&DHT11_PROC_GPOUP0,
		[DHT11_PROC_GPOUP1] 		= &&DHT11_PROC_GPOUP1,
		[DHT11_PROC_GPOUP2] 		= &&DHT11_PROC_GPOUP2,
		[DHT11_PROC_GPOUP3] 		= &&DHT11_PROC_GPOUP3,
		[DHT11_PROC_GPOUP4] 		= &&DHT11_PROC_GPOUP4,
		[DHT11_PROC_GPOUP5] 		= &&DHT11_PROC_GPOUP5,
		[DHT11_PROC_GPOUP6] 		= &&DHT11_PROC_GPOUP6,
		[DHT11_PROC_GPOUP7] 		= &&DHT11_PROC_GPOUP7,
		[DHT11_PROC_GPOUP8] 		= &&DHT11_PROC_GPOUP8,
		[DHT11_PROC_GPOUP10] 		= &&DHT11_PROC_GPOUP10,
		
		[DHT11_PROC_GPOUP12] 		= &&DHT11_PROC_GPOUP12,
		[DHT11_PROC_GPOUP14] 		= &&DHT11_PROC_GPOUP14,
		[DHT11_PROC_GPOUP15] 		= &&DHT11_PROC_GPOUP15,
		[DHT11_PROC_GPOUP16] 		= &&DHT11_PROC_GPOUP16,
		
		[DHT11_PROC_READ_BYTE_GROUP0] 		= &&DHT11_PROC_READ_BYTE_GROUP0,
		[DHT11_PROC_READ_BYTE_GROUP2] 		= &&DHT11_PROC_READ_BYTE_GROUP2,
		[DHT11_PROC_READ_BYTE_GROUP3] 		= &&DHT11_PROC_READ_BYTE_GROUP3,
		[DHT11_PROC_READ_BYTE_GROUP4] 		= &&DHT11_PROC_READ_BYTE_GROUP4,
		[DHT11_PROC_READ_BYTE_GROUP5] 		= &&DHT11_PROC_READ_BYTE_GROUP5,
		[DHT11_PROC_READ_ERROR] 			= &&DHT11_PROC_READ_ERROR,
		[DHT11_PROC_DELAY0] 			= &&DHT11_PROC_DELAY0,
		[DHT11_PROC_DELAY1] 			= &&DHT11_PROC_DELAY1,
		[DHT11_PROC_ENDP] 				= &&DHT11_PROC_ENDP,
	};
	
	goto *function[Device->Register.R15_PC];
DHT11_PROC_INIT:
	if( Device->Register.R6_Count != 0){ 
		Device->Register.R6_Count--;
	}else{
		Device->Register.R15_PC = DHT11_PROC_GPOUP0;
	}
	return;
DHT11_PROC_GPOUP0:
	if( Device->Start == 1){
		GPIO_SET_LOW(Device->GPIOx, Device->GPIO_Pin);//拉低电平至少18ms
		Device->Register.R6_Count = 1800;//10us一次 18ms/10us = 18000us/10us = 1800次
		Device->Register.R15_PC = DHT11_PROC_GPOUP1;
	}else{
		GPIO_SET_HIGH(Device->GPIOx, Device->GPIO_Pin);//拉高
	}
	return;
DHT11_PROC_GPOUP1:
	if( Device->Register.R6_Count == 0){
		Device->Register.R15_PC = DHT11_PROC_GPOUP2;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_GPOUP2:
	GPIO_SET_HIGH(Device->GPIOx, Device->GPIO_Pin);//拉高电平20~40us
	Device->Register.R6_Count = 2;//20us
	Device->Register.R15_PC = DHT11_PROC_GPOUP3;
	return;
DHT11_PROC_GPOUP3:
	if( Device->Register.R6_Count == 0){
		Device->Register.R6_Count = 10;//100us
		Device->Register.R15_PC = DHT11_PROC_GPOUP4;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_GPOUP4:
	if (GPIO_GET_STATE(Device->GPIOx, Device->GPIO_Pin) == 0){
		Device->Register.R6_Count = 10;//100us
		Device->Register.R15_PC = DHT11_PROC_GPOUP5;
	}else if( Device->Register.R6_Count == 0){
		GPIO_SET_HIGH(Device->GPIOx, Device->GPIO_Pin);//拉高
		Device->Register.R15_PC = DHT11_PROC_READ_ERROR;//出错了
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_GPOUP5:
	if (GPIO_GET_STATE(Device->GPIOx, Device->GPIO_Pin) == 1){
		Device->Register.R6_Count = 10;//100us
		Device->Register.R15_PC = DHT11_PROC_GPOUP6;
	}else if( Device->Register.R6_Count == 0){
		GPIO_SET_HIGH(Device->GPIOx, Device->GPIO_Pin);//拉高
		Device->Register.R15_PC = DHT11_PROC_READ_ERROR;//出错了
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_GPOUP6:
	if (GPIO_GET_STATE(Device->GPIOx, Device->GPIO_Pin) == 0){
		Device->Register.R6_Count = 10;//100us
		Device->Register.R15_PC = DHT11_PROC_GPOUP7;
	}else if( Device->Register.R6_Count == 0){
		Device->Register.R6_Count = 10;//100us
		Device->Register.R15_PC = DHT11_PROC_GPOUP7;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_GPOUP7:	
	Device->Register.R14_LR = DHT11_PROC_GPOUP8;
	Device->Register.R10_Mask = 0x80;
	Device->Register.R6_Count = 10;
	goto DHT11_PROC_READ_BYTE_GROUP0;
DHT11_PROC_GPOUP8:
	Device->humi_int = Device->Register.R0_Result;//湿度整数获取
	Device->Register.R14_LR = DHT11_PROC_GPOUP10;
	Device->Register.R10_Mask = 0x80;
	Device->Register.R6_Count = 10;
	goto DHT11_PROC_READ_BYTE_GROUP0;
DHT11_PROC_GPOUP10:	
	Device->humi_deci = Device->Register.R0_Result;//湿度整数获取
	Device->Register.R14_LR = DHT11_PROC_GPOUP12;
	Device->Register.R10_Mask = 0x80;
	Device->Register.R6_Count = 10;
	goto DHT11_PROC_READ_BYTE_GROUP0;
DHT11_PROC_GPOUP12:
	Device->temp_int = Device->Register.R0_Result;//湿度整数获取
	Device->Register.R14_LR = DHT11_PROC_GPOUP14;
	Device->Register.R10_Mask = 0x80;
	Device->Register.R6_Count = 10;
	goto DHT11_PROC_READ_BYTE_GROUP0;
DHT11_PROC_GPOUP14:	
	Device->temp_deci = Device->Register.R0_Result;//湿度整数获取
	Device->Register.R14_LR = DHT11_PROC_GPOUP15;
	Device->Register.R10_Mask = 0x80;
	Device->Register.R6_Count = 10;
	goto DHT11_PROC_READ_BYTE_GROUP0;
DHT11_PROC_GPOUP15:	
	Device->check_sum = Device->Register.R0_Result;//湿度整数获取
	Device->Register.R14_LR = DHT11_PROC_GPOUP16;
	Device->Register.R10_Mask = 0x80;
	Device->Register.R6_Count = 10;
	goto DHT11_PROC_READ_BYTE_GROUP0;
DHT11_PROC_GPOUP16:	
	if( Device->check_sum ==Device->humi_int+Device->humi_deci+Device->temp_int+Device->temp_deci){
		Device->Register.R9_Error = 0;
	}else{
		Device->Register.R9_Error = 1;
	}
	Device->Register.R15_PC = DHT11_PROC_DELAY0;
	return;
DHT11_PROC_READ_BYTE_GROUP0:
	if( GPIO_GET_STATE(Device->GPIOx, Device->GPIO_Pin) == 1){//从1bit开始，低电平变高电平，等待低电平结束
		Device->Register.R6_Count = 1;//只能是1才行
		Device->Register.R15_PC = DHT11_PROC_READ_BYTE_GROUP2;
	}else if( Device->Register.R6_Count == 0){
		Device->Register.R6_Count = 1;//只能是1才行
		Device->Register.R15_PC = DHT11_PROC_READ_BYTE_GROUP2;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_READ_BYTE_GROUP2:
	if( Device->Register.R6_Count == 0){//延时20us
		Device->Register.R15_PC = DHT11_PROC_READ_BYTE_GROUP3;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_READ_BYTE_GROUP3:
	if( GPIO_GET_STATE(Device->GPIOx, Device->GPIO_Pin) == 1){
		Device->Register.R0_Result |= Device->Register.R10_Mask;
	}else{
		Device->Register.R0_Result &=~Device->Register.R10_Mask;
	}
	goto DHT11_PROC_READ_BYTE_GROUP4;
DHT11_PROC_READ_BYTE_GROUP4:
	if( Device->Register.R10_Mask > 0){
		Device->Register.R10_Mask >>= 1;
		Device->Register.R6_Count = 10;
		Device->Register.R15_PC = DHT11_PROC_READ_BYTE_GROUP5;
	}else{
		goto *function[Device->Register.R14_LR];//读取错误的密码就在这里
	}
	return;
DHT11_PROC_READ_BYTE_GROUP5:
	if (GPIO_GET_STATE(Device->GPIOx, Device->GPIO_Pin) == 0){//从1bit开始，低电平变高电平，等待低电平结束
		Device->Register.R6_Count = 10;
		Device->Register.R15_PC = DHT11_PROC_READ_BYTE_GROUP0;
	}else if( Device->Register.R6_Count == 0){
		Device->Register.R6_Count = 10;
		Device->Register.R15_PC = DHT11_PROC_READ_BYTE_GROUP0;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_READ_ERROR:
	Device->humi_deci = 0;
	Device->humi_int  = 0;
	Device->temp_deci = 0;
	Device->temp_int  = 0;
	Device->Register.R9_Error = 1;
	Device->Register.R15_PC = DHT11_PROC_ENDP;
	return;
DHT11_PROC_DELAY0:
	Device->Register.R6_Count = 10;
	GPIO_SET_HIGH(Device->GPIOx, Device->GPIO_Pin);//拉高
	Device->Register.R15_PC = DHT11_PROC_DELAY1;
	return;
DHT11_PROC_DELAY1:
	if( Device->Register.R6_Count == 0){
		Device->Register.R15_PC = DHT11_PROC_ENDP;
	}else{
		Device->Register.R6_Count--;
	}
	return;
DHT11_PROC_ENDP:
	Device->Done  = 1;
	Device->Register.R6_Count = Device->Register.TimerTick;
	Device->Register.R15_PC = DHT11_PROC_INIT;
	return;
}


