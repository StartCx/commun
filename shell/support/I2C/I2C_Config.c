#include "I2C_Config.h"



I2C_M_Hardware_t I2C1_Master = {
	.I2Cx		= I2C1,
	.I2C_Speed	= 100000,//100k
	.Delay_time	= 0x1000,
	.Lock 		= CORE_UNLOCK,
	.PORT_SCL  	= GPIOB,
	.PIN_SCL   	= GPIO_Pin_6,
	.PORT_SDA  	= GPIOB,
	.PIN_SDA   	= GPIO_Pin_7,
	.Init		= I2C_M_Hardware_Config,
	.Open		= I2C_M_Hardware_Open,
	.Close		= I2C_M_Hardware_Close,
	.Endp 		= I2C_M_Hardware_Endp,
	.Result 	= I2C_M_Hardware_Result,
	.Set		= I2C_M_Hardware_Set,
	.Get		= I2C_M_Hardware_Get,
	.Write		= I2C_M_Hardware_Write,
	.Read		= I2C_M_Hardware_Read,
	.Detect		= I2C_M_Hardware_Detect,
};

I2C_M_Bitbang_t I2C2_Master = {
	.Timeout_Max = 20,
	.Register.R12_Lock = CORE_UNLOCK,
	.PORT_SCL  = GPIOB,
	.PIN_SCL   = GPIO_Pin_8,
	.PORT_SDA  = GPIOB,
	.PIN_SDA   = GPIO_Pin_9,
	.Init 		= I2C_M_Bitbang_Config,
	.Driver 	= I2C_M_Bitbang_Peripheral,
	.Close 		= I2C_M_Bitbang_Close,
	.Open 		= I2C_M_Bitbang_Open,
	.Endp 		= I2C_M_Bitbang_Endp,
	.Result 	= I2C_M_Bitbang_Result,
	.Detect		= I2C_M_Bitbang_Detect,
	.Set		= I2C_M_Bitbang_Set,
	.Get		= I2C_M_Bitbang_Get,
	.Write      = I2C_M_Bitbang_Write,
	.Read		= I2C_M_Bitbang_Read,
};

I2C_M_Software_t I2C3_Master = {
	.Delay_time	= 100,
	.Lock 		= CORE_UNLOCK,
	.PORT_SCL  	= GPIOB,
	.PIN_SCL   	= GPIO_Pin_4,
	.PORT_SDA  	= GPIOB,
	.PIN_SDA   	= GPIO_Pin_5,
	.Init		= I2C_M_Software_Config,
	.Open		= I2C_M_Software_Open,
	.Close		= I2C_M_Software_Close,
	.Endp 		= I2C_M_Software_Endp,
	.Result 	= I2C_M_Software_Result,
	.Set		= I2C_M_Software_Set,
	.Get		= I2C_M_Software_Get,
	.Write		= I2C_M_Software_Write,
	.Read		= I2C_M_Software_Read,
	.Detect		= I2C_M_Software_Detect,
};

I2C_S_Bitbang_t I2C4_Slave = {
	.Own_Addr   = 0x30,
	.Timeout_Max= 100,
	.Register.R12_Lock = CORE_UNLOCK,
	.PORT_SCL  	= GPIOA,
	.PIN_SCL   	= GPIO_Pin_15,
	.PORT_SDA  	= GPIOB,
	.PIN_SDA   	= GPIO_Pin_3,
	.Init 		= I2C_S_Bitbang_Config,
	.Driver 	= I2C_S_Bitbang_Peripheral,
};

void I2Cx_Peripheral(uint8_t Bus)
{
	switch(Bus){
		case I2C2_M_BITBANG: I2C2_Master.Driver(&I2C2_Master);	break;
		case I2C4_S_BITBANG: I2C4_Slave.Driver(&I2C4_Slave);	break;
		default:break;
	}
}

void I2Cx_Init(uint8_t Bus)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Init(&I2C1_Master);break;
		case I2C2_M_BITBANG:  I2C2_Master.Init(&I2C2_Master);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Init(&I2C3_Master);break;
		case I2C4_S_BITBANG:  I2C4_Slave.Init(&I2C4_Slave);  break;
		default:break;
	}
}


void I2Cx_Close(uint8_t Bus)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Close(&I2C1_Master);break;
		case I2C2_M_BITBANG:  I2C2_Master.Close(&I2C2_Master);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Close(&I2C3_Master);break;
		default:break;
	}
}

uint8_t I2Cx_Open(uint8_t Bus)
{
	switch(Bus){
		case I2C1_M_HARDWARE: return I2C1_Master.Open(&I2C1_Master);
		case I2C2_M_BITBANG:  return I2C2_Master.Open(&I2C2_Master);
		case I2C3_M_SOFTWARE: return I2C3_Master.Open(&I2C3_Master);
		default:return CORE_SUCCESS;
	}
}

uint8_t I2Cx_Endp(uint8_t Bus)
{
	switch(Bus){
		case I2C1_M_HARDWARE: return I2C1_Master.Endp(&I2C1_Master);
		case I2C2_M_BITBANG:  return I2C2_Master.Endp(&I2C2_Master);
		case I2C3_M_SOFTWARE: return I2C3_Master.Endp(&I2C3_Master);
		default:return CORE_SUCCESS;
	}
}

uint8_t I2Cx_Result(uint8_t Bus)
{
	switch(Bus){
		case I2C1_M_HARDWARE: return I2C1_Master.Result(&I2C1_Master);
		case I2C2_M_BITBANG:  return I2C2_Master.Result(&I2C2_Master);
		case I2C3_M_SOFTWARE: return I2C3_Master.Result(&I2C3_Master);
		default:return CORE_SUCCESS;
	}
}

void I2Cx_Set(uint8_t Bus, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *pData, uint16_t Size)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Set(&I2C1_Master,Dev_Addr,Reg_Addr,Reg_Size,pData,Size);break;
		case I2C2_M_BITBANG: I2C2_Master.Set(&I2C2_Master,Dev_Addr,Reg_Addr,Reg_Size,pData,Size);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Set(&I2C3_Master,Dev_Addr,Reg_Addr,Reg_Size,pData,Size);break;
		default:break;
	}
}

void I2Cx_Get(uint8_t Bus, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *pData, uint16_t Size)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Get(&I2C1_Master,Dev_Addr,Reg_Addr,Reg_Size,pData,Size);break;
		case I2C2_M_BITBANG:  I2C2_Master.Get(&I2C2_Master,Dev_Addr,Reg_Addr,Reg_Size,pData,Size);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Get(&I2C3_Master,Dev_Addr,Reg_Addr,Reg_Size,pData,Size);break;
		default:break;
	}
}

void I2Cx_Write(uint8_t Bus, uint8_t Dev_Addr, uint8_t *pData, uint16_t Size)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Write(&I2C1_Master,Dev_Addr,pData,Size);break;
		case I2C2_M_BITBANG:  I2C2_Master.Write(&I2C2_Master,Dev_Addr,pData,Size);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Write(&I2C3_Master,Dev_Addr,pData,Size);break;
		default:break;
	}
}


void I2Cx_Read(uint8_t Bus, uint8_t Dev_Addr, uint8_t *pData, uint16_t Size)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Read(&I2C1_Master,Dev_Addr,pData,Size);break;
		case I2C2_M_BITBANG:  I2C2_Master.Read(&I2C2_Master,Dev_Addr,pData,Size);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Read(&I2C3_Master,Dev_Addr,pData,Size);break;
		default:break;
	}
}

void I2Cx_Detect(uint8_t Bus, uint8_t Dev_Addr)
{
	switch(Bus){
		case I2C1_M_HARDWARE: I2C1_Master.Detect(&I2C1_Master,Dev_Addr);break;
		case I2C2_M_BITBANG:  I2C2_Master.Detect(&I2C2_Master,Dev_Addr);break;
		case I2C3_M_SOFTWARE: I2C3_Master.Detect(&I2C3_Master,Dev_Addr);break;
		default:break;
	}
}

