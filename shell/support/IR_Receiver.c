#include "IR_Receiver.h"



IR_receiver_t IR_Register = {
	.GPIOx  	= GPIOA,
	.GPIO_Pin 	= GPIO_Pin_15,
	.Timer = {
		.TickPeroid = 10,//10*10等于100us执行一次。
	},
};

void IR_RECV_Init(IR_receiver_t *IR_Register)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = IR_Register->GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(IR_Register->GPIOx, &GPIO_InitStructure);
	
	QueueEmpty(&IR_Register->Queue);
}


void IR_RECV_Driver(IR_receiver_t *IR_Register)
{
	enum
	{
		IR_PROC_BOOT_CODE_GROUP0,
		IR_PROC_BOOT_CODE_GROUP1,
		IR_PROC_BOOT_CODE_GROUP2,
		IR_PROC_RECV_DATA_GROUP0,
		IR_PROC_RECV_DATA_GROUP1,
		IR_PROC_RECV_DATA_GROUP2,
		IR_PROC_ERROR_WAIT,
		IR_PROC_SUM,
	};
	static const void *function[IR_PROC_SUM] = {
		[IR_PROC_BOOT_CODE_GROUP0] 	= &&IR_PROC_BOOT_CODE_GROUP0,
		[IR_PROC_BOOT_CODE_GROUP1] 	= &&IR_PROC_BOOT_CODE_GROUP1,
		[IR_PROC_BOOT_CODE_GROUP2] 	= &&IR_PROC_BOOT_CODE_GROUP2,
		[IR_PROC_RECV_DATA_GROUP0] 	= &&IR_PROC_RECV_DATA_GROUP0,
		[IR_PROC_RECV_DATA_GROUP1] 	= &&IR_PROC_RECV_DATA_GROUP1,
		[IR_PROC_RECV_DATA_GROUP2] 	= &&IR_PROC_RECV_DATA_GROUP2,
		[IR_PROC_ERROR_WAIT] 		= &&IR_PROC_ERROR_WAIT,
	};
	goto *function[IR_Register->Register.R15_PC];

IR_PROC_BOOT_CODE_GROUP0:
	if( GPIO_GET_STATE(IR_Register->GPIOx, IR_Register->GPIO_Pin) == 0){
		IR_Register->Register.TimerTick = 0;
		IR_Register->Register.R1_Index  = 0;
		IR_Register->Register.R10_Mask  = 0x80000000;
		IR_Register->Register.R15_PC = IR_PROC_BOOT_CODE_GROUP1;
	}else{
		IR_Register->Register.R15_PC = IR_PROC_BOOT_CODE_GROUP0;
	}
	return;
IR_PROC_BOOT_CODE_GROUP1:
	if( GPIO_GET_STATE(IR_Register->GPIOx, IR_Register->GPIO_Pin) == 1){//100us一次
		IR_Register->Leader_Code[0] = IR_Register->Register.TimerTick;
		
		if( IR_Register->Register.TimerTick >= 40 && IR_Register->Register.TimerTick <= 110){//引导码低电平时间正确
			IR_Register->Register.TimerTick = 0;
			IR_Register->Register.R15_PC = IR_PROC_BOOT_CODE_GROUP2;
		}else{//引导码低电平时间错误
			IR_Register->Register.R14_LR = 1;
			IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
			IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
		}
	}else if( IR_Register->Register.TimerTick > 110){
		IR_Register->Register.R14_LR = 2;
		IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
		IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
	}else{
		IR_Register->Register.R15_PC = IR_PROC_BOOT_CODE_GROUP1;
		IR_Register->Register.TimerTick++;
	}
	return;
IR_PROC_BOOT_CODE_GROUP2:
	if( GPIO_GET_STATE(IR_Register->GPIOx, IR_Register->GPIO_Pin) == 0){//100us一次
		IR_Register->Leader_Code[1] = IR_Register->Register.TimerTick;
		if( IR_Register->Register.TimerTick >= 30 && IR_Register->Register.TimerTick <= 65){//引导码低电平时间正确
			IR_Register->Register.TimerTick = 0;
			IR_Register->Register.R15_PC = IR_PROC_RECV_DATA_GROUP0;
		}else if( IR_Register->Register.TimerTick >= 10 && IR_Register->Register.TimerTick < 30){//重复码
			//is 重复码
			IR_Register->Register.R14_LR = 3;
			IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
			
			IR_Register->Register.R0_Result = 0x00FF00FF;
			IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 24) & 0xFF;
			QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
			IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 16) & 0xFF;
			QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
			IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 8) & 0xFF;
			QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
			IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 0) & 0xFF;
			QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
		
			IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
		}else{//引导码低电平时间错误
			IR_Register->Register.R14_LR = 4;
			IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
			IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
		}
	}else if( IR_Register->Register.TimerTick > 65){
		IR_Register->Register.R14_LR = 5;
		IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
		IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
	}else{
		IR_Register->Register.R15_PC = IR_PROC_BOOT_CODE_GROUP2;
		IR_Register->Register.TimerTick++;
	}
	return;
IR_PROC_RECV_DATA_GROUP0:
	if( GPIO_GET_STATE(IR_Register->GPIOx, IR_Register->GPIO_Pin) == 1){//100us一次
		IR_Register->Code_Low_time[IR_Register->Register.R1_Index] = IR_Register->Register.TimerTick;
		if( IR_Register->Register.TimerTick <= 10){//引导码低电平时间正确
			IR_Register->Register.TimerTick = 0;
			IR_Register->Register.R15_PC = IR_PROC_RECV_DATA_GROUP1;
		}else{//引导码低电平时间错误
			IR_Register->Register.R14_LR = 6;
			IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
			IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
		}
	}else if( IR_Register->Register.TimerTick > 10){
		IR_Register->Register.R14_LR = 7;
		IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
		IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
	}else{
		IR_Register->Register.R15_PC = IR_PROC_RECV_DATA_GROUP0;
		IR_Register->Register.TimerTick++;
	}
	return;
IR_PROC_RECV_DATA_GROUP1:
	if( GPIO_GET_STATE(IR_Register->GPIOx, IR_Register->GPIO_Pin) == 0){//100us一次
		IR_Register->Code_High_time[IR_Register->Register.R1_Index] = IR_Register->Register.TimerTick;
		if( IR_Register->Register.TimerTick <= 9){
			IR_Register->Register.R0_Result &= ~IR_Register->Register.R10_Mask;//0
			IR_Register->Register.R15_PC = IR_PROC_RECV_DATA_GROUP2;
		}else if( IR_Register->Register.TimerTick > 9 && IR_Register->Register.TimerTick <= 20){//1
			IR_Register->Register.R0_Result |=  IR_Register->Register.R10_Mask;//1
			IR_Register->Register.R15_PC = IR_PROC_RECV_DATA_GROUP2;
		}else{//引导码低电平时间错误
			IR_Register->Register.R14_LR = 8;
			IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
			IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
		}
	}else if( IR_Register->Register.TimerTick > 30){
		IR_Register->Register.R14_LR = 9;
		IR_Register->Register.R6_Count = IR_Register->Register.TimerTick;
		IR_Register->Register.R15_PC = IR_PROC_ERROR_WAIT;
	}else{
		IR_Register->Register.R15_PC = IR_PROC_RECV_DATA_GROUP1;
		IR_Register->Register.TimerTick++;
	}
	return;
IR_PROC_RECV_DATA_GROUP2:
	IR_Register->Register.TimerTick = 0;
	IR_Register->Register.R10_Mask >>= 1;
	if( IR_Register->Register.R10_Mask > 0){
		IR_Register->Register.R1_Index++;
		goto IR_PROC_RECV_DATA_GROUP0;
	}else{
		IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 24) & 0xFF;
		QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
		IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 16) & 0xFF;
		QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
		IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 8) & 0xFF;
		QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
		IR_Register->Register.R2_cin = (IR_Register->Register.R0_Result >> 0) & 0xFF;
		QueueDataIn(&IR_Register->Queue, &IR_Register->Register.R2_cin);
		
		IR_Register->Register.R15_PC =  IR_PROC_ERROR_WAIT;
	}
	return;
IR_PROC_ERROR_WAIT:
	if( GPIO_GET_STATE(IR_Register->GPIOx, IR_Register->GPIO_Pin) == 1){//100us一次
		if( IR_Register->Register.TimerTick++ >= 10){//引导码低电平时间正确
			IR_Register->Register.TimerTick = 0;
			IR_Register->Register.R15_PC = IR_PROC_BOOT_CODE_GROUP0;
		}
	}else{
		IR_Register->Register.TimerTick = 0;
	}
	return;
}









