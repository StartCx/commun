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
	.Delay_time	= 10,
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




