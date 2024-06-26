#include "DS18B20.h"

DS18B20_Driver_t DS18B20_Dev = {
	.GPIOx = GPIOB,
	.GPIO_Pin = GPIO_Pin_0,
	.Register.TimerTick = 500000,//5s
	.Register.R6_Count = 0,
};


void DS18B20_Init(DS18B20_Driver_t *DS18B20_Device)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = DS18B20_Device->GPIO_Pin;				//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DS18B20_Device->GPIOx, &GPIO_InitStructure);
}


void DS18B20_Start(DS18B20_Driver_t *DS18B20_Device)
{		
	DS18B20_Device->Start = 1;
	DS18B20_Device->Done  = 0;
}

void DS18B20_Stop(DS18B20_Driver_t *DS18B20_Device)
{		
	DS18B20_Device->Start = 0;
	DS18B20_Device->Done  = 0;
}


void DS18B20_Get_Temp(DS18B20_Driver_t *DS18B20_Device)
{
	enum
	{
		DS18B20_PROC_DELAY,
		DS18B20_HANDLE_INIT,
		DS18B20_HANDLE_GROUP0,
		DS18B20_HANDLE_GROUP1,
		DS18B20_HANDLE_GROUP2,
		DS18B20_HANDLE_GROUP3,
		DS18B20_HANDLE_GROUP4,
		DS18B20_HANDLE_GROUP5,
		DS18B20_HANDLE_GROUP6,
		DS18B20_HANDLE_GROUP7,
		DS18B20_HANDLE_GROUP8,
		DS18B20_HANDLE_GROUP9,
		DS18B20_HANDLE_GROUP10,
		DS18B20_HANDLE_GROUP10_1,
		DS18B20_HANDLE_GROUP11,
		
		DS18B20_RESET_GPOUP0,
		DS18B20_RESET_GPOUP1,
		DS18B20_RESET_GPOUP2,
		
		DS18B20_ACK_GPOUP0,
		DS18B20_ACK_GPOUP1,
		
		DS18B20_WRITE_BYTE_GROUP0,
		DS18B20_WRITE_BYTE_GROUP1,
		DS18B20_WRITE_BYTE_GROUP2,
		DS18B20_WRITE_BYTE_GROUP3,
		DS18B20_WRITE_BYTE_GROUP4,
		
		DS18B20_READ_BYTE_GROUP0,
		DS18B20_READ_BYTE_GROUP1,
		DS18B20_READ_BYTE_GROUP2,
		DS18B20_READ_BYTE_GROUP3,
		DS18B20_READ_BYTE_GROUP4,
		
		DS18B20_PROC_TASK_SUM,
	};
	static const void *function[DS18B20_PROC_TASK_SUM] = {
		[DS18B20_PROC_DELAY] 		= &&DS18B20_PROC_DELAY,
		[DS18B20_HANDLE_INIT] 		= &&DS18B20_HANDLE_INIT,
		[DS18B20_HANDLE_GROUP0] 	= &&DS18B20_HANDLE_GROUP0,
		[DS18B20_HANDLE_GROUP1] 	= &&DS18B20_HANDLE_GROUP1,
		[DS18B20_HANDLE_GROUP2] 	= &&DS18B20_HANDLE_GROUP2,
		[DS18B20_HANDLE_GROUP3] 	= &&DS18B20_HANDLE_GROUP3,
		[DS18B20_HANDLE_GROUP4] 	= &&DS18B20_HANDLE_GROUP4,
		[DS18B20_HANDLE_GROUP5] 	= &&DS18B20_HANDLE_GROUP5,
		[DS18B20_HANDLE_GROUP6] 	= &&DS18B20_HANDLE_GROUP6,
		[DS18B20_HANDLE_GROUP7] 	= &&DS18B20_HANDLE_GROUP7,
		[DS18B20_HANDLE_GROUP8] 	= &&DS18B20_HANDLE_GROUP8,
		[DS18B20_HANDLE_GROUP9] 	= &&DS18B20_HANDLE_GROUP9,
		[DS18B20_HANDLE_GROUP10] 	= &&DS18B20_HANDLE_GROUP10,
		[DS18B20_HANDLE_GROUP10_1] 	= &&DS18B20_HANDLE_GROUP10_1,
		[DS18B20_HANDLE_GROUP11] 	= &&DS18B20_HANDLE_GROUP11,
		
		[DS18B20_RESET_GPOUP0] 		= &&DS18B20_RESET_GPOUP0,
		[DS18B20_RESET_GPOUP1] 		= &&DS18B20_RESET_GPOUP1,
		[DS18B20_RESET_GPOUP2] 		= &&DS18B20_RESET_GPOUP2,
		
		[DS18B20_ACK_GPOUP0] 		= &&DS18B20_ACK_GPOUP0,
		[DS18B20_ACK_GPOUP1] 		= &&DS18B20_ACK_GPOUP1,
		
		[DS18B20_WRITE_BYTE_GROUP0] = &&DS18B20_WRITE_BYTE_GROUP0,
		[DS18B20_WRITE_BYTE_GROUP1] = &&DS18B20_WRITE_BYTE_GROUP1,
		[DS18B20_WRITE_BYTE_GROUP2] = &&DS18B20_WRITE_BYTE_GROUP2,
		[DS18B20_WRITE_BYTE_GROUP3] = &&DS18B20_WRITE_BYTE_GROUP3,
		[DS18B20_WRITE_BYTE_GROUP4] = &&DS18B20_WRITE_BYTE_GROUP4,
		
		[DS18B20_READ_BYTE_GROUP0] = &&DS18B20_READ_BYTE_GROUP0,
		[DS18B20_READ_BYTE_GROUP1] = &&DS18B20_READ_BYTE_GROUP1,
		[DS18B20_READ_BYTE_GROUP2] = &&DS18B20_READ_BYTE_GROUP2,
		[DS18B20_READ_BYTE_GROUP3] = &&DS18B20_READ_BYTE_GROUP3,
		[DS18B20_READ_BYTE_GROUP4] = &&DS18B20_READ_BYTE_GROUP4,
	};
	goto *function[DS18B20_Device->Register.R15_PC];
DS18B20_PROC_DELAY:
	if( DS18B20_Device->Register.R6_Count != 0){ 
		DS18B20_Device->Register.R6_Count--;
	}else{
		DS18B20_Device->Register.R15_PC = DS18B20_HANDLE_INIT;
	}
	return;
DS18B20_HANDLE_INIT:
	if( DS18B20_Device->Start == 1){
		DS18B20_Device->Register.R15_PC = DS18B20_HANDLE_GROUP0;
	}
	return;
DS18B20_HANDLE_GROUP0:
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP1;
	DS18B20_Device->Register.R15_PC = DS18B20_RESET_GPOUP0;
	return;
DS18B20_HANDLE_GROUP1:
	DS18B20_Device->Register.R3_cout = 0xCC;
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP2;
	DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP0;
	return;
DS18B20_HANDLE_GROUP2:
	DS18B20_Device->Register.R3_cout = 0x44;
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP3;
	DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP0;
	return;
DS18B20_HANDLE_GROUP3:
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP4;
	DS18B20_Device->Register.R15_PC = DS18B20_RESET_GPOUP0;
	return;
DS18B20_HANDLE_GROUP4:
	DS18B20_Device->Register.R3_cout = 0xCC;
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP5;
	DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP0;
	return;
DS18B20_HANDLE_GROUP5:
	DS18B20_Device->Register.R3_cout = 0xbe;
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP6;
	DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP0;
	return;
DS18B20_HANDLE_GROUP6:
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP7;
	DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP0;
	return;
DS18B20_HANDLE_GROUP7:
	DS18B20_Device->TL = DS18B20_Device->Register.R2_cin;
	DS18B20_Device->Register.R14_LR = DS18B20_HANDLE_GROUP8;
	DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP0;
	return;
DS18B20_HANDLE_GROUP8:
	DS18B20_Device->TH = DS18B20_Device->Register.R2_cin;
	DS18B20_Device->Register.R6_Count = 0;
	DS18B20_Device->Register.R15_PC = DS18B20_HANDLE_GROUP9;
	return;
DS18B20_HANDLE_GROUP9:
	DS18B20_Device->Register.R0_Result = DS18B20_Device->TH;
	DS18B20_Device->Register.R0_Result <<= 8;
	DS18B20_Device->Register.R0_Result += DS18B20_Device->TL;
	DS18B20_Device->Register.R15_PC = DS18B20_HANDLE_GROUP10;
	return;
DS18B20_HANDLE_GROUP10:
	DS18B20_Device->Register.R0_Result *= 5;
	DS18B20_Device->Register.R0_Result /= 8;
	DS18B20_Device->Temp = DS18B20_Device->Register.R0_Result;
	DS18B20_Device->Register.R15_PC = DS18B20_HANDLE_GROUP10_1;
	return;
DS18B20_HANDLE_GROUP10_1:
	if( DS18B20_Device->Temp > 1000){
		DS18B20_Device->Temp = 0;
		DS18B20_Device->Register.R9_Error = 1;
	}else{
		DS18B20_Device->Register.R9_Error = 0;
	}
	DS18B20_Device->Register.R15_PC = DS18B20_HANDLE_GROUP11;
	return;
DS18B20_HANDLE_GROUP11:
	DS18B20_Device->Done  = 1;
	DS18B20_Device->Register.R6_Count = DS18B20_Device->Register.TimerTick;
	DS18B20_Device->Register.R15_PC = DS18B20_PROC_DELAY;
	return;
DS18B20_RESET_GPOUP0:
	DS18B20_Device->Register.R6_Count = 1;
	GPIO_SET_LOW(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
	DS18B20_Device->Register.R15_PC = DS18B20_RESET_GPOUP1;
	return;
DS18B20_RESET_GPOUP1:
	if( DS18B20_Device->Register.R6_Count >= 75){
		GPIO_SET_HIGH(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
		DS18B20_Device->Register.R6_Count = 1;
		DS18B20_Device->Register.R15_PC = DS18B20_RESET_GPOUP2;
	}else{
		DS18B20_Device->Register.R6_Count++;
	}
	return;
DS18B20_RESET_GPOUP2:
	if( DS18B20_Device->Register.R6_Count >= 5){
		DS18B20_Device->Register.R6_Count = 1;
		DS18B20_Device->Register.R15_PC = DS18B20_ACK_GPOUP0;
	}else{
		DS18B20_Device->Register.R6_Count++;
	}
	return;
DS18B20_ACK_GPOUP0:
	if( GPIO_GET_STATE( DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin) == 1 && DS18B20_Device->Register.R6_Count <20){
		DS18B20_Device->Register.R6_Count++;
	}else{
		DS18B20_Device->Register.R6_Count = 1;
		DS18B20_Device->Register.R15_PC = DS18B20_ACK_GPOUP1;
	}
	return;
DS18B20_ACK_GPOUP1:
	if( GPIO_GET_STATE( DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin) == 0 && DS18B20_Device->Register.R6_Count <24){
		DS18B20_Device->Register.R6_Count++;
	}else{
		DS18B20_Device->Register.R6_Count = 1;
		DS18B20_Device->Register.R15_PC = DS18B20_Device->Register.R14_LR;
	}
	return;
DS18B20_WRITE_BYTE_GROUP0:
	DS18B20_Device->Register.R10_Mask = 0x01;
	DS18B20_Device->Register.R15_PC =  DS18B20_WRITE_BYTE_GROUP1;
	return;
DS18B20_WRITE_BYTE_GROUP1:
	if( DS18B20_Device->Register.R10_Mask <= 0x80){
		DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP2;
	}else{
		DS18B20_Device->Register.R15_PC = DS18B20_Device->Register.R14_LR;
	}
	return;
DS18B20_WRITE_BYTE_GROUP2:
	DS18B20_Device->Register.R6_Count = 0;
	GPIO_SET_LOW(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
	if( DS18B20_Device->Register.R3_cout & DS18B20_Device->Register.R10_Mask){
		DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP3;
	}else{
		DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP4;
	}
	return;
DS18B20_WRITE_BYTE_GROUP3:
	GPIO_SET_HIGH(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
	if( DS18B20_Device->Register.R6_Count > 3){
		DS18B20_Device->Register.R10_Mask<<=1;
		DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP1;
	}else{
		DS18B20_Device->Register.R6_Count++;
	}
	return;
DS18B20_WRITE_BYTE_GROUP4:
	if( DS18B20_Device->Register.R6_Count > 3){
		GPIO_SET_HIGH(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
		DS18B20_Device->Register.R10_Mask<<=1;
		DS18B20_Device->Register.R15_PC = DS18B20_WRITE_BYTE_GROUP1;
	}else{
		DS18B20_Device->Register.R6_Count++;
	}
	return;
DS18B20_READ_BYTE_GROUP0:
	DS18B20_Device->Register.R10_Mask = 0x01;
	DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP1;
	return;
DS18B20_READ_BYTE_GROUP1:
	if( DS18B20_Device->Register.R10_Mask <= 0x80){
		DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP2;
	}else{
		DS18B20_Device->Register.R15_PC = DS18B20_Device->Register.R14_LR;
	}
	return;
DS18B20_READ_BYTE_GROUP2:
	GPIO_SET_LOW(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
	DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP3;
	return;
DS18B20_READ_BYTE_GROUP3:
	GPIO_SET_HIGH(DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin);
	if( GPIO_GET_STATE( DS18B20_Device->GPIOx, DS18B20_Device->GPIO_Pin)){
		DS18B20_Device->Register.R2_cin |=  DS18B20_Device->Register.R10_Mask;  // 将特定位设置为高
	}else{
		DS18B20_Device->Register.R2_cin &= ~DS18B20_Device->Register.R10_Mask;  // 将特定位设置为低
	}
	DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP4;
	DS18B20_Device->Register.R6_Count = 0;
	return;
DS18B20_READ_BYTE_GROUP4:
	if( DS18B20_Device->Register.R6_Count > 3){
		DS18B20_Device->Register.R10_Mask<<=1;
		DS18B20_Device->Register.R15_PC = DS18B20_READ_BYTE_GROUP1;
	}else{
		DS18B20_Device->Register.R6_Count++;
	}
	return;
}


