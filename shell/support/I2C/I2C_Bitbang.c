#include "I2C_Bitbang.h"


enum
{
	 I2C_TASK_READY 	= 0x00,
	 I2C_SEND_REG_DATA 	= 0x01,
	 I2C_RECV_REG_DATA 	= 0x02,
	 I2C_SEND_DATA 		= 0x03,
	 I2C_RECV_DATA 		= 0x04,
	 I2C_DETECT_ADDR	= 0x05,
};


void I2C_M_Bitbang_Config(I2C_M_Bitbang_t *I2C_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SCL;
	GPIO_Init(I2C_Driver->PORT_SCL, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SDA;
	GPIO_Init(I2C_Driver->PORT_SDA, &GPIO_InitStructure);
	
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
}


uint8_t I2C_M_Bitbang_Open(I2C_M_Bitbang_t *I2C_Driver)
{
	if( I2C_Driver->Register.R12_Lock == CORE_UNLOCK && I2C_Driver->Mode == I2C_TASK_READY && I2C_Driver->Register.R15_PC == 0){
		I2C_Driver->Register.R12_Lock = CORE_LOCK;
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void I2C_M_Bitbang_Close(I2C_M_Bitbang_t *I2C_Driver)
{
	I2C_Driver->Register.R12_Lock = CORE_UNLOCK;
}



void I2C_M_Bitbang_Set(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size)
{
	I2C_Driver->Dev_Addr = Dev_Addr;
	I2C_Driver->Reg_Addr = Reg_Addr;
	I2C_Driver->Reg_Byte = Reg_Size;
	I2C_Driver->pData 	= value;
	I2C_Driver->Size 	= Size;
	I2C_Driver->Mode 	= I2C_SEND_REG_DATA;
}


void I2C_M_Bitbang_Get(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size)
{
	I2C_Driver->Dev_Addr = Dev_Addr;
	I2C_Driver->Reg_Addr = Reg_Addr;
	I2C_Driver->Reg_Byte = Reg_Size;
	I2C_Driver->pData 	= value;
	I2C_Driver->Size 	= Size;
	I2C_Driver->Mode 	= I2C_RECV_REG_DATA;
}


void I2C_M_Bitbang_Write(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size)
{
	I2C_Driver->Dev_Addr = Dev_Addr;
	I2C_Driver->pData 	= value;
	I2C_Driver->Size 	= Size;
	I2C_Driver->Mode 	= I2C_SEND_DATA;
}

void I2C_M_Bitbang_Read(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size)
{
	I2C_Driver->Dev_Addr= Dev_Addr;
	I2C_Driver->pData 	= value;
	I2C_Driver->Size 	= Size;
	I2C_Driver->Mode 	= I2C_RECV_DATA;
}

void I2C_M_Bitbang_Detect(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr)
{
	I2C_Driver->Dev_Addr = Dev_Addr;
	I2C_Driver->Mode 	 = I2C_DETECT_ADDR;
}

uint8_t I2C_M_Bitbang_Endp(I2C_M_Bitbang_t *I2C_Driver)
{
	if( I2C_Driver->Register.R15_PC == 0 && I2C_Driver->Mode == I2C_TASK_READY){
		return CORE_DONE;
	}else{
		return CORE_RUNNING;
	}
}

uint8_t I2C_M_Bitbang_Result(I2C_M_Bitbang_t *I2C_Driver)
{
	return I2C_Driver->Register.R9_Error;
}

void I2C_M_Bitbang_Peripheral(I2C_M_Bitbang_t *I2C_Driver)
{
	enum
	{
		I2C_STATE_READY	= 0,
		I2C_STATE_SELF_CHECK,
		I2C_STATE_READY_PARAM,
		I2C_Start_SCL_SDA_HIGH,
		I2C_Start_SDA_LOW,
		I2C_Start_SCL_LOW,
		
		I2C_WRITE_SET_DEV_ADDR,
		I2C_WRITE_DEV_ADDR_SET_SDA,
		I2C_WRITE_DEV_ADDR_SCL_LOW,
		I2C_DEV_ADDR_WAITACK_SCL_HIGH_SDA_HIGH,
		I2C_DEV_ADDR_WAITACK_GET_SDA_SCL_LOW,
		I2C_DEV_ADDR_ACK_MODE_JUMP,
		
		I2C_WRITE_SET_REG_ADDR_HIGH_8BIT,
		I2C_WRITE_SET_REG_ADDR_LOW_8BIT,
		I2C_WRITE_REG_ADDR_SET_SDA,
		I2C_WRITE_REG_ADDR_SCL_LOW,
		I2C_REG_ADDR_WAITACK_SCL_HIGH_SDA_HIGH,
		I2C_REG_ADDR_WAITACK_GET_SDA_SCL_LOW,
		I2C_REG_ADDR_ACK_MODE_JUMP,
		
		I2C_WRITE_SET_VALUE,
		I2C_WRITE_SET_VALUE_1,
		I2C_WRITE_VALUE_SET_SDA,
		I2C_WRITE_VALUE_SCL_LOW,
		I2C_WRITE_VALUE_WAITACK_SCL_HIGH_SDA_HIGH,
		I2C_WRITE_VALUE_WAITACK_GET_SDA_SCL_LOW,
		
		I2C_Restart_SCL_SDA_HIGH,
		I2C_Restart_SDA_LOW,
		I2C_Restart_SCL_LOW,
		
		I2C_READ_SET_DEV_ADDR,
		I2C_READ_DEV_ADDR_SET_SDA,
		I2C_READ_DEV_ADDR_SCL_LOW,
		I2C_DEV_ADDR_READ_WAITACK_SCL_HIGH_SDA_HIGH,
		I2C_DEV_ADDR_READ_WAITACK_GET_SDA_SCL_LOW,
		
		I2C_READ_DATA_LENGTH_ZERO,
		I2C_READ_DATA_SCL_HIGH,
		I2C_READ_DATA_GET_SDA_SCL_LOW,
		I2C_READ_DATA_GET_SDA_SCL_LOW_1,
		I2C_READ_DATA_GET_VALUE,
		
		I2C_NACK_SDA_HIGH,
		I2C_NACK_SCL_HIGH,
		I2C_NACK_SCL_LOW,
		
		I2C_ACK_SDA_LOW,
		I2C_ACK_SCL_HIGH,
		I2C_ACK_SCL_LOW,
		I2C_ACK_SDA_HIGH,
		
		I2C_STOP_SDA_LOW_SCL_LOW,
		I2C_STOP_SDA_LOW_SCL_HIGH,
		I2C_STOP_SDA_HIGH,
		I2C_STATE_SUM,
	};
	static const void *function[I2C_STATE_SUM] = {
		[I2C_STATE_READY] 								= &&I2C_STATE_READY,
		[I2C_STATE_SELF_CHECK] 							= &&I2C_STATE_SELF_CHECK,
		[I2C_STATE_READY_PARAM] 						= &&I2C_STATE_READY_PARAM,
		[I2C_Start_SCL_SDA_HIGH] 						= &&I2C_Start_SCL_SDA_HIGH,
		[I2C_Start_SDA_LOW] 							= &&I2C_Start_SDA_LOW,
		[I2C_Start_SCL_LOW] 							= &&I2C_Start_SCL_LOW,
		[I2C_WRITE_SET_DEV_ADDR] 						= &&I2C_WRITE_SET_DEV_ADDR,
		[I2C_WRITE_DEV_ADDR_SET_SDA] 					= &&I2C_WRITE_DEV_ADDR_SET_SDA,
		[I2C_WRITE_DEV_ADDR_SCL_LOW] 					= &&I2C_WRITE_DEV_ADDR_SCL_LOW,
		[I2C_DEV_ADDR_WAITACK_SCL_HIGH_SDA_HIGH] 		= &&I2C_DEV_ADDR_WAITACK_SCL_HIGH_SDA_HIGH,
		[I2C_DEV_ADDR_WAITACK_GET_SDA_SCL_LOW] 			= &&I2C_DEV_ADDR_WAITACK_GET_SDA_SCL_LOW,
		[I2C_DEV_ADDR_ACK_MODE_JUMP] 					= &&I2C_DEV_ADDR_ACK_MODE_JUMP,
		
		[I2C_WRITE_SET_REG_ADDR_HIGH_8BIT] 				= &&I2C_WRITE_SET_REG_ADDR_HIGH_8BIT,
		[I2C_WRITE_SET_REG_ADDR_LOW_8BIT] 				= &&I2C_WRITE_SET_REG_ADDR_LOW_8BIT,
		
		[I2C_WRITE_REG_ADDR_SET_SDA] 					= &&I2C_WRITE_REG_ADDR_SET_SDA,
		[I2C_WRITE_REG_ADDR_SCL_LOW] 					= &&I2C_WRITE_REG_ADDR_SCL_LOW,
		[I2C_REG_ADDR_WAITACK_SCL_HIGH_SDA_HIGH] 		= &&I2C_REG_ADDR_WAITACK_SCL_HIGH_SDA_HIGH,
		[I2C_REG_ADDR_WAITACK_GET_SDA_SCL_LOW] 			= &&I2C_REG_ADDR_WAITACK_GET_SDA_SCL_LOW,
		[I2C_REG_ADDR_ACK_MODE_JUMP] 					= &&I2C_REG_ADDR_ACK_MODE_JUMP,
		
		[I2C_WRITE_SET_VALUE] 							= &&I2C_WRITE_SET_VALUE,
		[I2C_WRITE_SET_VALUE_1] 						= &&I2C_WRITE_SET_VALUE_1,
		[I2C_WRITE_VALUE_SET_SDA] 						= &&I2C_WRITE_VALUE_SET_SDA,
		[I2C_WRITE_VALUE_SCL_LOW] 						= &&I2C_WRITE_VALUE_SCL_LOW,
		[I2C_WRITE_VALUE_WAITACK_SCL_HIGH_SDA_HIGH] 	= &&I2C_WRITE_VALUE_WAITACK_SCL_HIGH_SDA_HIGH,
		[I2C_WRITE_VALUE_WAITACK_GET_SDA_SCL_LOW] 		= &&I2C_WRITE_VALUE_WAITACK_GET_SDA_SCL_LOW,
		[I2C_Restart_SCL_SDA_HIGH] 						= &&I2C_Restart_SCL_SDA_HIGH,
		[I2C_Restart_SDA_LOW] 							= &&I2C_Restart_SDA_LOW,
		[I2C_Restart_SCL_LOW] 							= &&I2C_Restart_SCL_LOW,
		[I2C_READ_SET_DEV_ADDR] 						= &&I2C_READ_SET_DEV_ADDR,
		[I2C_READ_DEV_ADDR_SET_SDA] 					= &&I2C_READ_DEV_ADDR_SET_SDA,
		[I2C_READ_DEV_ADDR_SCL_LOW] 					= &&I2C_READ_DEV_ADDR_SCL_LOW,
		[I2C_DEV_ADDR_READ_WAITACK_SCL_HIGH_SDA_HIGH] 	= &&I2C_DEV_ADDR_READ_WAITACK_SCL_HIGH_SDA_HIGH,
		[I2C_DEV_ADDR_READ_WAITACK_GET_SDA_SCL_LOW] 	= &&I2C_DEV_ADDR_READ_WAITACK_GET_SDA_SCL_LOW,
		[I2C_READ_DATA_LENGTH_ZERO] 					= &&I2C_READ_DATA_LENGTH_ZERO,
		[I2C_READ_DATA_SCL_HIGH] 						= &&I2C_READ_DATA_SCL_HIGH,
		[I2C_READ_DATA_GET_SDA_SCL_LOW] 				= &&I2C_READ_DATA_GET_SDA_SCL_LOW,
		[I2C_READ_DATA_GET_SDA_SCL_LOW_1] 				= &&I2C_READ_DATA_GET_SDA_SCL_LOW_1,
		[I2C_READ_DATA_GET_VALUE] 						= &&I2C_READ_DATA_GET_VALUE,
		[I2C_NACK_SDA_HIGH] 							= &&I2C_NACK_SDA_HIGH,
		[I2C_NACK_SCL_HIGH] 							= &&I2C_NACK_SCL_HIGH,
		[I2C_NACK_SCL_LOW] 								= &&I2C_NACK_SCL_LOW,
		[I2C_ACK_SDA_LOW] 								= &&I2C_ACK_SDA_LOW,
		[I2C_ACK_SCL_HIGH] 								= &&I2C_ACK_SCL_HIGH,
		[I2C_ACK_SCL_LOW] 								= &&I2C_ACK_SCL_LOW,
		[I2C_ACK_SDA_HIGH] 								= &&I2C_ACK_SDA_HIGH,
		
		[I2C_STOP_SDA_LOW_SCL_LOW] 						= &&I2C_STOP_SDA_LOW_SCL_LOW,
		[I2C_STOP_SDA_LOW_SCL_HIGH] 					= &&I2C_STOP_SDA_LOW_SCL_HIGH,
		[I2C_STOP_SDA_HIGH] 							= &&I2C_STOP_SDA_HIGH,
	};
	
	goto *function[I2C_Driver->Register.R15_PC];
	
I2C_STATE_READY:
	if( I2C_Driver->Mode != I2C_TASK_READY){
		I2C_Driver->Register.R15_PC = I2C_STATE_SELF_CHECK;
	}
	return;
I2C_STATE_SELF_CHECK:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 || GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Register.R9_Error = BUS_ERROR;
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_HIGH;
	}else{
		I2C_Driver->Register.R15_PC = I2C_STATE_READY_PARAM;
	}
	return;
I2C_STATE_READY_PARAM:
	I2C_Driver->Register.R1_Index = 0;
	I2C_Driver->Register.R9_Error = NO_ERROR;
	if( I2C_Driver->Mode == I2C_RECV_DATA){
		I2C_Driver->Register.R15_PC = I2C_Restart_SCL_SDA_HIGH;
	}else{
		I2C_Driver->Register.R15_PC = I2C_Start_SCL_SDA_HIGH;
	}
	return;
I2C_Start_SCL_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_Start_SDA_LOW;
	return;
I2C_Start_SDA_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_Start_SCL_LOW;
	return;
I2C_Start_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_WRITE_SET_DEV_ADDR;
	return;
I2C_WRITE_SET_DEV_ADDR:
	I2C_Driver->Register.R10_Mask = 0x80;
	I2C_Driver->Register.R11_ch = I2C_Driver->Dev_Addr & ~0x01;
	I2C_Driver->Register.R15_PC = I2C_WRITE_DEV_ADDR_SET_SDA;
	return;
I2C_WRITE_DEV_ADDR_SET_SDA:
	if (I2C_Driver->Register.R11_ch & I2C_Driver->Register.R10_Mask){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}else{
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_WRITE_DEV_ADDR_SCL_LOW;
	return;
I2C_WRITE_DEV_ADDR_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R10_Mask >>= 1;
	if( I2C_Driver->Register.R10_Mask > 0){
		I2C_Driver->Register.R15_PC = I2C_WRITE_DEV_ADDR_SET_SDA;
	}else{
		I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_WAITACK_SCL_HIGH_SDA_HIGH;
	}
	return;
I2C_DEV_ADDR_WAITACK_SCL_HIGH_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_WAITACK_GET_SDA_SCL_LOW;
	I2C_Driver->Timeout = I2C_Driver[0].Timeout_Max;
	return;
I2C_DEV_ADDR_WAITACK_GET_SDA_SCL_LOW:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
		I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_ACK_MODE_JUMP;
	}else if( I2C_Driver->Timeout == 0){
		I2C_Driver->Register.R9_Error = DEV_ADDR_NO_ACK;
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}else{
		I2C_Driver->Timeout--;
	}
	return;
I2C_DEV_ADDR_ACK_MODE_JUMP:
	if( I2C_Driver->Mode == I2C_DETECT_ADDR){
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}else if( I2C_Driver->Mode == I2C_SEND_DATA){
		I2C_Driver->Register.R15_PC = I2C_WRITE_SET_VALUE;
	}else{
		if( I2C_Driver->Reg_Byte == 2){
			I2C_Driver->Register.R15_PC = I2C_WRITE_SET_REG_ADDR_HIGH_8BIT;
		}else{
			I2C_Driver->Register.R15_PC = I2C_WRITE_SET_REG_ADDR_LOW_8BIT;
		}
	}
	return;
I2C_WRITE_SET_REG_ADDR_HIGH_8BIT:
	I2C_Driver->Register.R10_Mask = 0x80;	
	I2C_Driver->Register.R11_ch = (uint8_t)(I2C_Driver->Reg_Addr>>8);
	I2C_Driver->Register.R14_LR = I2C_WRITE_SET_REG_ADDR_LOW_8BIT;
	I2C_Driver->Register.R15_PC = I2C_WRITE_REG_ADDR_SET_SDA;
	return;
I2C_WRITE_SET_REG_ADDR_LOW_8BIT:
	I2C_Driver->Register.R10_Mask = 0x80;	
	I2C_Driver->Register.R11_ch = (uint8_t)(I2C_Driver->Reg_Addr&0x00FF);
	I2C_Driver->Register.R14_LR = I2C_REG_ADDR_ACK_MODE_JUMP;
	I2C_Driver->Register.R15_PC = I2C_WRITE_REG_ADDR_SET_SDA;
	return;
I2C_WRITE_REG_ADDR_SET_SDA:
	if (I2C_Driver->Register.R11_ch & I2C_Driver->Register.R10_Mask){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}else{
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_WRITE_REG_ADDR_SCL_LOW;
	return;
I2C_WRITE_REG_ADDR_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R10_Mask >>= 1;
	if( I2C_Driver->Register.R10_Mask > 0){
		I2C_Driver->Register.R15_PC = I2C_WRITE_REG_ADDR_SET_SDA;
	}else{
		I2C_Driver->Register.R15_PC = I2C_REG_ADDR_WAITACK_SCL_HIGH_SDA_HIGH;
	}
	return;
I2C_REG_ADDR_WAITACK_SCL_HIGH_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_REG_ADDR_WAITACK_GET_SDA_SCL_LOW;
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	return;
I2C_REG_ADDR_WAITACK_GET_SDA_SCL_LOW:
	if(GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
		I2C_Driver->Register.R15_PC = I2C_Driver->Register.R14_LR;
	}else if( I2C_Driver->Timeout == 0){
		I2C_Driver->Register.R9_Error = REG_ADDR_NO_ACK;
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}else{
		I2C_Driver->Timeout--;
	}
	return;
I2C_REG_ADDR_ACK_MODE_JUMP:
	if( I2C_Driver->Mode == I2C_RECV_REG_DATA){
		I2C_Driver->Register.R15_PC = I2C_Restart_SCL_SDA_HIGH;
	}else if( I2C_Driver->Mode == I2C_SEND_REG_DATA){
		I2C_Driver->Register.R15_PC = I2C_WRITE_SET_VALUE;
	}
	return;
I2C_WRITE_SET_VALUE:
	I2C_Driver->Register.R10_Mask = 0x80;
	if( I2C_Driver->Register.R1_Index < I2C_Driver->Size){
		I2C_Driver->Register.R15_PC = I2C_WRITE_SET_VALUE_1;
	}else{
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}
	return;
I2C_WRITE_SET_VALUE_1:
	I2C_Driver->Register.R11_ch = I2C_Driver->pData[I2C_Driver->Register.R1_Index];
	I2C_Driver->Register.R1_Index++;
	I2C_Driver->Register.R15_PC = I2C_WRITE_VALUE_SET_SDA;
	return;
I2C_WRITE_VALUE_SET_SDA:
	if (I2C_Driver->Register.R11_ch & I2C_Driver->Register.R10_Mask){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}else{
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_WRITE_VALUE_SCL_LOW;
	return;
I2C_WRITE_VALUE_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL,I2C_Driver->PIN_SCL);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R10_Mask >>= 1;
	if( I2C_Driver->Register.R10_Mask > 0){
		I2C_Driver->Register.R15_PC = I2C_WRITE_VALUE_SET_SDA;
	}else{
		I2C_Driver->Register.R15_PC = I2C_WRITE_VALUE_WAITACK_SCL_HIGH_SDA_HIGH;
	}
	return;
I2C_WRITE_VALUE_WAITACK_SCL_HIGH_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_WRITE_VALUE_WAITACK_GET_SDA_SCL_LOW;
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	return;
I2C_WRITE_VALUE_WAITACK_GET_SDA_SCL_LOW:
	if(GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
		I2C_Driver->Register.R15_PC = I2C_WRITE_SET_VALUE;
	}else if( I2C_Driver->Timeout == 0){
		I2C_Driver->Register.R9_Error = WR_VALUE_NO_ACK;
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}else{
		I2C_Driver->Timeout--;
	}
	return;
I2C_Restart_SCL_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_Restart_SDA_LOW;
	return;
I2C_Restart_SDA_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_Restart_SCL_LOW;
	return;
I2C_Restart_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_READ_SET_DEV_ADDR;
I2C_READ_SET_DEV_ADDR:
	I2C_Driver->Register.R10_Mask = 0x80;
	I2C_Driver->Register.R11_ch = I2C_Driver->Dev_Addr | 0x01;
	I2C_Driver->Register.R15_PC = I2C_READ_DEV_ADDR_SET_SDA;
	return;
I2C_READ_DEV_ADDR_SET_SDA:
	if (I2C_Driver->Register.R11_ch & I2C_Driver->Register.R10_Mask){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}else{
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	}
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_READ_DEV_ADDR_SCL_LOW;
	return;
I2C_READ_DEV_ADDR_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R10_Mask >>= 1;
	if( I2C_Driver->Register.R10_Mask > 0){
		I2C_Driver->Register.R15_PC = I2C_READ_DEV_ADDR_SET_SDA;
	}else{
		I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_READ_WAITACK_SCL_HIGH_SDA_HIGH;
	}
	return;
I2C_DEV_ADDR_READ_WAITACK_SCL_HIGH_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_READ_WAITACK_GET_SDA_SCL_LOW;
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	return;
I2C_DEV_ADDR_READ_WAITACK_GET_SDA_SCL_LOW:
	I2C_Driver->Register.R10_Mask  = 0x80;
	if(GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
		I2C_Driver->Register.R15_PC = I2C_READ_DATA_LENGTH_ZERO;
	}else if( I2C_Driver->Timeout == 0){
		I2C_Driver->Register.R9_Error = DEV_ADDR_NO_ACK;
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}else{
		I2C_Driver->Timeout--;
	}
	return;
I2C_READ_DATA_LENGTH_ZERO:
	if( I2C_Driver->Register.R1_Index < I2C_Driver->Size){
		I2C_Driver->Register.R15_PC = I2C_READ_DATA_SCL_HIGH;
	}else{
		I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	}
	return;
I2C_READ_DATA_SCL_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_READ_DATA_GET_SDA_SCL_LOW;
	return;
I2C_READ_DATA_GET_SDA_SCL_LOW:
	if(GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Register.R11_ch &= ~I2C_Driver->Register.R10_Mask;  // ���ض�λ����Ϊ��//7
	}else{
		I2C_Driver->Register.R11_ch |=  I2C_Driver->Register.R10_Mask;  // ���ض�λ����Ϊ��//7
	}
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_READ_DATA_GET_SDA_SCL_LOW_1;
	return;
I2C_READ_DATA_GET_SDA_SCL_LOW_1:
	I2C_Driver->Register.R10_Mask >>= 1;
	if( I2C_Driver->Register.R10_Mask > 0){
		I2C_Driver->Register.R15_PC = I2C_READ_DATA_SCL_HIGH;
	}else{
		I2C_Driver->Register.R15_PC = I2C_READ_DATA_GET_VALUE;
	}
	return;
I2C_READ_DATA_GET_VALUE:
	I2C_Driver->Register.R10_Mask  = 0x80;
	I2C_Driver->pData[I2C_Driver->Register.R1_Index] = I2C_Driver->Register.R11_ch;
	I2C_Driver->Register.R1_Index++;
	if( I2C_Driver->Register.R1_Index < I2C_Driver->Size){
		I2C_Driver->Register.R15_PC = I2C_ACK_SDA_LOW;
	}else{
		I2C_Driver->Register.R15_PC = I2C_NACK_SDA_HIGH;
	}
	return;
I2C_ACK_SDA_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_ACK_SCL_HIGH;
	return;
I2C_ACK_SCL_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_ACK_SCL_LOW;
	return;
I2C_ACK_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_ACK_SDA_HIGH;
	return;
I2C_ACK_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_READ_DATA_SCL_HIGH;
	return;
I2C_NACK_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_NACK_SCL_HIGH;
	return;
I2C_NACK_SCL_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_NACK_SCL_LOW;
	return;
I2C_NACK_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_LOW;
	return;
I2C_STOP_SDA_LOW_SCL_LOW:
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_STOP_SDA_LOW_SCL_HIGH;
	return;
I2C_STOP_SDA_LOW_SCL_HIGH:
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	I2C_Driver->Register.R15_PC = I2C_STOP_SDA_HIGH;
	return;
I2C_STOP_SDA_HIGH:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_STATE_READY;
	I2C_Driver->Mode  = I2C_TASK_READY;
	return;
}






/****************************************************************************************************************************************************************/

void I2C_S_Bitbang_Config(I2C_S_Bitbang_t *I2C_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SCL;
	GPIO_Init(I2C_Driver->PORT_SCL, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SDA;
	GPIO_Init(I2C_Driver->PORT_SDA, &GPIO_InitStructure);
	
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	
	for(int i = 0; i < 256;i++){
		I2C_Driver->Array_Reg[i] = i;
	}
}

void I2C_S_Bitbang_Peripheral(I2C_S_Bitbang_t *I2C_Driver)
{
	enum
	{
		I2C_WAIT_START_GROUP0 = 0,
		I2C_WAIT_START_GROUP1,
		I2C_WAIT_START_GROUP2,
		I2C_WAIT_SCL_TO_HIGH,
		I2C_GET_DEV_ADDR_SCL_HIGH,
		I2C_GET_DEV_ADDR_SCL_LOW = 5,
		I2C_WAIT_DEV_ACK_SCL_SDA_HIGH,
		I2C_DEV_ADDR_COMPARE_ACK,
	
		I2C_DEV_ACK_END_WAIT,
		I2C_DEV_ADDR_SET_OR_GET,
		I2C_DEV_REG_ADDR_GET_SCL_HIGH = 10,
		I2C_DEV_REG_ADDR_GET_SDA_GET,
		I2C_DEV_REG_ADDR_GET_SCL_LOW,
		I2C_WAIT_REG_ACK_SCL_SDA_HIGH,
		I2C_SLAVE_REG_ADDR_ACK,
		
		I2C_SLAVE_WAIT_GET_ACK = 15,
		I2C_SLAVE_GET_DATA_SCL_HIGH,
		I2C_SLAVE_GET_DATA_GET_SDA,
		I2C_RESTART_OR_GET_DATA,
		I2C_SLAVE_GET_DATA_SCL_LOW,
		I2C_WAIT_SLAVE_DATA_ACK = 20,
		I2C_SLAVE_DATA_ACK_SCL_LOW,
		I2C_SLAVE_WAIT_MASTER_GET_ACK,
		
		I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA,
		I2C_SLAVE_SET_DATA_WAIT_SCL_HIGH,
		I2C_SLAVE_SET_DATA_WAIT_SCL_LOW = 25,
		I2C_SLAVE_WAIT_MASTER_ACK_SCL_SDA_HIGH,
		I2C_SLAVE_WAIT_MASTER_ACK_GET_SDA,
		I2C_SLAVE_CONTINUE_SET_DATA,
		
		I2C_WAIT_STOP_GROUP0,
		I2C_WAIT_STOP_GROUP1 = 30,
		I2C_WAIT_STOP_GROUP2,
		
		I2C_CALL_PROC_GROUP_ENDP,
		I2C_STATE_SUM,
	};
	
	static const void *function[I2C_STATE_SUM] = {
		[I2C_WAIT_START_GROUP0] = &&I2C_WAIT_START_GROUP0,
		[I2C_WAIT_START_GROUP1] = &&I2C_WAIT_START_GROUP1,
		[I2C_WAIT_START_GROUP2] = &&I2C_WAIT_START_GROUP2,
		[I2C_WAIT_SCL_TO_HIGH] = &&I2C_WAIT_SCL_TO_HIGH,
		[I2C_GET_DEV_ADDR_SCL_HIGH] = &&I2C_GET_DEV_ADDR_SCL_HIGH,
		[I2C_GET_DEV_ADDR_SCL_LOW] = &&I2C_GET_DEV_ADDR_SCL_LOW,
		[I2C_WAIT_DEV_ACK_SCL_SDA_HIGH] = &&I2C_WAIT_DEV_ACK_SCL_SDA_HIGH,
		[I2C_DEV_ADDR_COMPARE_ACK] = &&I2C_DEV_ADDR_COMPARE_ACK,
		
		[I2C_DEV_ACK_END_WAIT] = &&I2C_DEV_ACK_END_WAIT,
		[I2C_DEV_ADDR_SET_OR_GET] = &&I2C_DEV_ADDR_SET_OR_GET,
		[I2C_DEV_REG_ADDR_GET_SCL_HIGH] = &&I2C_DEV_REG_ADDR_GET_SCL_HIGH,
		[I2C_DEV_REG_ADDR_GET_SDA_GET] = &&I2C_DEV_REG_ADDR_GET_SDA_GET,
		[I2C_DEV_REG_ADDR_GET_SCL_LOW] = &&I2C_DEV_REG_ADDR_GET_SCL_LOW,
		[I2C_WAIT_REG_ACK_SCL_SDA_HIGH] = &&I2C_WAIT_REG_ACK_SCL_SDA_HIGH,
		[I2C_SLAVE_REG_ADDR_ACK] = &&I2C_SLAVE_REG_ADDR_ACK,
	
		[I2C_SLAVE_WAIT_GET_ACK] = &&I2C_SLAVE_WAIT_GET_ACK,
		[I2C_SLAVE_GET_DATA_SCL_HIGH] = &&I2C_SLAVE_GET_DATA_SCL_HIGH,
		[I2C_SLAVE_GET_DATA_GET_SDA] = &&I2C_SLAVE_GET_DATA_GET_SDA,
		[I2C_RESTART_OR_GET_DATA] = &&I2C_RESTART_OR_GET_DATA,
		[I2C_SLAVE_GET_DATA_SCL_LOW] = &&I2C_SLAVE_GET_DATA_SCL_LOW,
		[I2C_WAIT_SLAVE_DATA_ACK] = &&I2C_WAIT_SLAVE_DATA_ACK,
		[I2C_SLAVE_DATA_ACK_SCL_LOW] = &&I2C_SLAVE_DATA_ACK_SCL_LOW,
		[I2C_SLAVE_WAIT_MASTER_GET_ACK] = &&I2C_SLAVE_WAIT_MASTER_GET_ACK,
		
		[I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA] = &&I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA,
		[I2C_SLAVE_SET_DATA_WAIT_SCL_HIGH] = &&I2C_SLAVE_SET_DATA_WAIT_SCL_HIGH,
		[I2C_SLAVE_SET_DATA_WAIT_SCL_LOW] = &&I2C_SLAVE_SET_DATA_WAIT_SCL_LOW,
		[I2C_SLAVE_WAIT_MASTER_ACK_SCL_SDA_HIGH] = &&I2C_SLAVE_WAIT_MASTER_ACK_SCL_SDA_HIGH,
		[I2C_SLAVE_WAIT_MASTER_ACK_GET_SDA] = &&I2C_SLAVE_WAIT_MASTER_ACK_GET_SDA,
		[I2C_SLAVE_CONTINUE_SET_DATA] = &&I2C_SLAVE_CONTINUE_SET_DATA,
		
		[I2C_WAIT_STOP_GROUP0] = &&I2C_WAIT_STOP_GROUP0,
		[I2C_WAIT_STOP_GROUP1] = &&I2C_WAIT_STOP_GROUP1,
		[I2C_WAIT_STOP_GROUP2] = &&I2C_WAIT_STOP_GROUP2,
		
		[I2C_CALL_PROC_GROUP_ENDP] = &&I2C_CALL_PROC_GROUP_ENDP,
	};
	
	goto *function[I2C_Driver->Register.R15_PC];

I2C_WAIT_START_GROUP0:
	I2C_Driver->Register.R8_Flag = 0;
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Register.R15_PC = I2C_WAIT_START_GROUP1;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_WAIT_START_GROUP0;
	}
	return;
I2C_WAIT_START_GROUP1:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Register.R15_PC = I2C_WAIT_START_GROUP2;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_WAIT_START_GROUP1;
	}
	return;
I2C_WAIT_START_GROUP2:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Register.R10_Mask = 0x80;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_SCL_TO_HIGH;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_WAIT_SCL_TO_HIGH:		
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1){//��ʱ������SDA״̬
		I2C_Driver->Register.R15_PC = I2C_GET_DEV_ADDR_SCL_HIGH;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_GET_DEV_ADDR_SCL_HIGH:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Dev_Addr |= I2C_Driver->Register.R10_Mask;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Dev_Addr &= ~I2C_Driver->Register.R10_Mask;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
		return;
	}
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	I2C_Driver->Register.R15_PC = I2C_GET_DEV_ADDR_SCL_LOW;
	return;
I2C_GET_DEV_ADDR_SCL_LOW:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0){//��ʱ������SDA״̬
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R10_Mask >>= 1;
		if( I2C_Driver->Register.R10_Mask > 0){
			I2C_Driver->Register.R15_PC = I2C_WAIT_SCL_TO_HIGH;
		}else{
			I2C_Driver->Register.R15_PC = I2C_WAIT_DEV_ACK_SCL_SDA_HIGH;
		}
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_WAIT_DEV_ACK_SCL_SDA_HIGH://�豸��ַ�ȴ�״̬�ж�
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_COMPARE_ACK;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_DEV_ADDR_COMPARE_ACK:
	if( I2C_Driver->Dev_Addr == I2C_Driver->Own_Addr || I2C_Driver->Dev_Addr == I2C_Driver->Own_Addr+1){//���豸��ַ�ж�
		GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);//Ӧ��
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_DEV_ACK_END_WAIT;
	}else{
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP0;
	}
	return;

I2C_DEV_ACK_END_WAIT://Ӧ�����
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		I2C_Driver->Register.R10_Mask = 0x80;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_DEV_ADDR_SET_OR_GET;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_DEV_ADDR_SET_OR_GET:
	if( I2C_Driver->Dev_Addr == I2C_Driver->Own_Addr){
		I2C_Driver->Register.R15_PC = I2C_DEV_REG_ADDR_GET_SCL_HIGH;
	}else{
		if( I2C_Driver->Register.R8_Flag == 0){
			I2C_Driver->Reg_Addr = 0;
		}
		I2C_Driver->Register.R15_PC = I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA;
	}
	return;
I2C_DEV_REG_ADDR_GET_SCL_HIGH://��ʼ��������
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1){
		I2C_Driver->Register.R15_PC = I2C_DEV_REG_ADDR_GET_SDA_GET;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_DEV_REG_ADDR_GET_SDA_GET:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Reg_Addr |= I2C_Driver->Register.R10_Mask;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Reg_Addr &= ~I2C_Driver->Register.R10_Mask;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
		return;
	}
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	I2C_Driver->Register.R15_PC = I2C_DEV_REG_ADDR_GET_SCL_LOW;
	return;
I2C_DEV_REG_ADDR_GET_SCL_LOW:
	I2C_Driver->Register.R8_Flag = 1;
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0){//��ʱ������SDA״̬
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R10_Mask >>= 1;
		if( I2C_Driver->Register.R10_Mask > 0){
			I2C_Driver->Register.R15_PC = I2C_DEV_REG_ADDR_GET_SCL_HIGH;
		}else{
			I2C_Driver->Register.R15_PC = I2C_WAIT_REG_ACK_SCL_SDA_HIGH;
		}
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_WAIT_REG_ACK_SCL_SDA_HIGH:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Register.R15_PC = I2C_SLAVE_REG_ADDR_ACK;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_SLAVE_REG_ADDR_ACK:	
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	I2C_Driver->Register.R15_PC = I2C_SLAVE_WAIT_GET_ACK;
	return;
I2C_SLAVE_WAIT_GET_ACK:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);//������ȡ����Ӧ�𣬽�SCL������
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R10_Mask = 0x80;
		if( I2C_Driver->Dev_Addr == I2C_Driver->Own_Addr){	//�����������ݡ�
			I2C_Driver->Register.R15_PC = I2C_SLAVE_GET_DATA_SCL_HIGH;
		}else{											//�����������ݡ�
			I2C_Driver->Register.R15_PC = I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA;
		}
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_SLAVE_GET_DATA_SCL_HIGH://����������׼���������ݣ��ߵ�ƽ��ȡ���ݡ�����һ����������������������ݣ����������·��Ϳ�ʼ�źš�
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1){
		I2C_Driver->Register.R15_PC = I2C_SLAVE_GET_DATA_GET_SDA;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_SLAVE_GET_DATA_GET_SDA:
	I2C_Driver->Register.R4_Status = GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);	
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Register.R2_cin |= I2C_Driver->Register.R10_Mask;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Register.R2_cin &= ~I2C_Driver->Register.R10_Mask;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
		return;
	}
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	I2C_Driver->Register.R15_PC = I2C_RESTART_OR_GET_DATA;
	return;
I2C_RESTART_OR_GET_DATA:
	if( I2C_Driver->Register.R10_Mask == 0x80 && I2C_Driver->Register.R4_Status == 1){
		if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 &&  GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
			I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
			I2C_Driver->Register.R15_PC = I2C_WAIT_START_GROUP1;//�豸��ַ���ճɹ�ֻ������·��1�ǽ������������ݣ�2�����¿�ʼ�������������ݡ�
			return;
		}
	}
	I2C_Driver->Register.R15_PC = I2C_SLAVE_GET_DATA_SCL_LOW;
	return;
I2C_SLAVE_GET_DATA_SCL_LOW:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0){//��ʱ������SDA״̬
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R10_Mask >>= 1;
		if( I2C_Driver->Register.R10_Mask > 0){
			I2C_Driver->Register.R15_PC = I2C_SLAVE_GET_DATA_SCL_HIGH;
		}else{
			I2C_Driver->Register.R15_PC = I2C_WAIT_SLAVE_DATA_ACK;
		}
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
		I2C_Driver->Register.R15_PC = I2C_RESTART_OR_GET_DATA;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//�˳�ʱ�Ƿ�������ж�����
	}
	return;
I2C_WAIT_SLAVE_DATA_ACK://�Ĵ���ֵ�ȴ�״̬�ж�
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Register.R15_PC = I2C_SLAVE_DATA_ACK_SCL_LOW;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_SLAVE_DATA_ACK_SCL_LOW://�Ĵ���ֵ��ȡ�ɹ���Ӧ������SDA��
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
	I2C_Driver->Register.R15_PC = I2C_SLAVE_WAIT_MASTER_GET_ACK;
	return;
I2C_SLAVE_WAIT_MASTER_GET_ACK://Ӧ�����
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		I2C_Driver->Array_Reg[I2C_Driver->Reg_Addr] = I2C_Driver->Register.R2_cin;
		if( I2C_Driver->Reg_Addr < 255){
			I2C_Driver->Reg_Addr++;
		}else{
			I2C_Driver->Reg_Addr = 0;
		}
		I2C_Driver->Register.R10_Mask = 0x80;
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_SLAVE_GET_DATA_SCL_HIGH;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA://��ʼ��������
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0){//˵����ʱ�ǵ͵�ƽ������MSB���λ
		if (I2C_Driver->Array_Reg[ I2C_Driver->Reg_Addr ] & I2C_Driver->Register.R10_Mask){
			GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		}else{
			GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		}
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_SLAVE_SET_DATA_WAIT_SCL_HIGH;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//�˳�ʱ�Ƿ�������ж�����
	}
	return;
I2C_SLAVE_SET_DATA_WAIT_SCL_HIGH://�ȴ�SCL����
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1){//�ȴ�SCL����
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_SLAVE_SET_DATA_WAIT_SCL_LOW;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//�˳�ʱ�Ƿ�������ж�����
	}
	return;
I2C_SLAVE_SET_DATA_WAIT_SCL_LOW:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0){//��ʱ������SDA״̬
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R10_Mask >>= 1;
		if( I2C_Driver->Register.R10_Mask > 0){
			I2C_Driver->Register.R15_PC = I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA;
		}else{
			GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);//�ӻ����ߣ��ȴ�Ӧ��
			I2C_Driver->Register.R15_PC = I2C_SLAVE_WAIT_MASTER_ACK_SCL_SDA_HIGH;
		}
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//�˳�ʱ�Ƿ�������ж�����
	}
	return;
I2C_SLAVE_WAIT_MASTER_ACK_SCL_SDA_HIGH://˵������һ��byte���ݽ��������Ӧ���벻Ӧ��
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1){//����ʱ��ȡ�ӳٶ�ȡ����״̬
		I2C_Driver->Register.R15_PC = I2C_SLAVE_WAIT_MASTER_ACK_GET_SDA;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//�˳�ʱ�Ƿ�������ж�����
	}
	return;
I2C_SLAVE_WAIT_MASTER_ACK_GET_SDA:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//��Ӧ��ֱ�������ж���ʼ
	}else{
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_SLAVE_CONTINUE_SET_DATA;//Ӧ��ɹ����ȴ��������ͻ�ȡ����
	}
	return;
I2C_SLAVE_CONTINUE_SET_DATA://�ȴ�SCL�������ͣ�������������
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0){//��ʱ������SDA״̬
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		if( I2C_Driver->Reg_Addr < 255){
			I2C_Driver->Reg_Addr++;
		}else{
			I2C_Driver->Reg_Addr = 0;
		}
		
		I2C_Driver->Register.R10_Mask = 0x80;
		I2C_Driver->Register.R15_PC = I2C_SLAVE_SET_DATA_SCL_LOW_SET_SDA;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;//�˳�ʱ�Ƿ�������ж�����
	}
	return;
I2C_WAIT_STOP_GROUP0:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP1;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_WAIT_STOP_GROUP1:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP2;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP0;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP0;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}
	return;
I2C_WAIT_STOP_GROUP2:
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 1 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_CALL_PROC_GROUP_ENDP;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP0;
	}else if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 && GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1){
		I2C_Driver->Timeout = I2C_Driver->Timeout_Max;
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP0;
	}else if( I2C_Driver->Timeout > 0){
		I2C_Driver->Timeout--;
	}else{
		I2C_Driver->Register.R15_PC = I2C_WAIT_STOP_GROUP0;
	}
	return;
I2C_CALL_PROC_GROUP_ENDP:
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	I2C_Driver->Register.R15_PC = I2C_WAIT_START_GROUP0;//�˳�ʱ�Ƿ�������ж�����
	return;
}

