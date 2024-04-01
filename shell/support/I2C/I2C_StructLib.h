#ifndef __I2C_STRUCTLIB_H
#define __I2C_STRUCTLIB_H



#include "core.h"


enum 
{
	NO_ERROR		= 0x00,
    DEV_ADDR_NO_ACK = 0x10,
	REG_ADDR_NO_ACK = 0x20,
	WR_VALUE_NO_ACK	= 0x30,
	BUS_ERROR		= 0x40,
};


typedef struct I2C_M_Bitbang_t
{
	uint8_t Mode;
	uint8_t Dev_Addr;
	uint16_t Reg_Addr;
	uint8_t  Reg_Byte;
	uint8_t *pData;
	uint16_t Size;
	uint16_t Timeout;
	uint16_t Timeout_Max;
	
	GPIO_TypeDef	*PORT_SCL;	//端口
	uint16_t 		PIN_SCL;	//引脚
	GPIO_TypeDef	*PORT_SDA;	//端口
	uint16_t 		PIN_SDA;	//引脚
	
	CPU_RegisterClass_t Register;
	//方法
	void (*Init)(struct I2C_M_Bitbang_t *I2C_Driver);
	void (*Driver)(struct I2C_M_Bitbang_t *I2C_Driver);
	void (*Close)(struct I2C_M_Bitbang_t *I2C_Driver);
	uint8_t (*Open)(struct I2C_M_Bitbang_t *I2C_Driver);
	uint8_t (*Endp)(struct I2C_M_Bitbang_t *I2C_Driver);
	uint8_t (*Result)(struct I2C_M_Bitbang_t *I2C_Driver);
	void (*Detect)(struct I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr);
	void (*Set)(struct I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
	void (*Get)(struct I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr,uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
	void (*Write)(struct I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
	void (*Read)(struct I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);

}I2C_M_Bitbang_t;


typedef struct I2C_S_Bitbang_t
{
	uint8_t Own_Addr;
	uint8_t Dev_Addr;
	uint16_t Reg_Addr;
	uint16_t Timeout;
	uint16_t Timeout_Max;
	uint8_t  Array_Reg[256];
	
	GPIO_TypeDef	*PORT_SCL;	//端口
	uint16_t 		PIN_SCL;	//引脚
	GPIO_TypeDef	*PORT_SDA;	//端口
	uint16_t 		PIN_SDA;	//引脚
	CPU_RegisterClass_t Register;
	//方法
	void (*Init)(struct I2C_S_Bitbang_t *Dev);
	void (*Driver)(struct I2C_S_Bitbang_t *Dev);
}I2C_S_Bitbang_t;


typedef struct I2C_M_Software_t
{
	GPIO_TypeDef	*PORT_SCL;	//端口
	uint16_t 		PIN_SCL;	//引脚
	GPIO_TypeDef	*PORT_SDA;	//端口
	uint16_t 		PIN_SDA;	//引脚
	uint16_t 		Delay_time;	//延迟时间
	uint16_t 		Delay_cnt;	//延迟计数
	uint8_t 		Mask;
	uint8_t 		Ack_state;
	uint8_t 		ReadData;
	uint16_t 		Index;
	uint8_t 		Lock;
	uint8_t 		Ret;
	
	//方法
	void (*Init)(struct I2C_M_Software_t *I2C_Driver);
	void (*Close)(struct I2C_M_Software_t *I2C_Driver);
	uint8_t (*Endp)(struct I2C_M_Software_t *I2C_Driver);
	uint8_t (*Result)(struct I2C_M_Software_t *I2C_Driver);
	uint8_t (*Open)(struct I2C_M_Software_t *I2C_Driver);
	void (*Detect)(struct I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr);
	void (*Set)(struct I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
	void (*Get)(struct I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr,uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
	void (*Write)(struct I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
	void (*Read)(struct I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
}I2C_M_Software_t;


typedef struct I2C_M_Hardware_t
{
	I2C_TypeDef 	*I2Cx;
	uint32_t 		I2C_Speed;
	GPIO_TypeDef	*PORT_SCL;	//端口
	uint16_t 		PIN_SCL;	//引脚
	GPIO_TypeDef	*PORT_SDA;	//端口
	uint16_t 		PIN_SDA;	//引脚
	uint16_t 		Delay_time;	//延迟时间	
	uint16_t 		Delay_cnt;	//延迟计数
	uint16_t 		Index;
	uint8_t 		Lock;
	uint8_t 		Ret;
	//方法
	void (*Init)(struct I2C_M_Hardware_t *I2C_Driver);
	void (*Close)(struct I2C_M_Hardware_t *I2C_Driver);
	uint8_t (*Endp)(struct I2C_M_Hardware_t *I2C_Driver);
	uint8_t (*Result)(struct I2C_M_Hardware_t *I2C_Driver);
	uint8_t (*Open)(struct I2C_M_Hardware_t *I2C_Driver);
	void (*Detect)(struct I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr);
	void (*Set)(struct I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
	void (*Get)(struct I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr,uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
	void (*Write)(struct I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
	void (*Read)(struct I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
}I2C_M_Hardware_t;


#endif
